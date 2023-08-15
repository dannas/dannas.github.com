---
layout: post
title: Import Polar trainingdata to Strava
---

<!-- excerpt start -->
When you sync data between your Polar account and Strava you don't get your full training history imported. I used some online tools and wrote a python script to to a full export
<!-- excerpt end -->

## Getting your data

Polar has an API but it doesn't support retrieving files older than one month. So I did a bulk export by following the instructions on https://support.polar.com/en/how-to-download-all-your-data-from-polar-flow: You click a "download your data" link and wait for an email with a zip file containing all your training history. My exported zip file contained 2625 json files.

```
λ ls data | grep -oP "^\w+" | sort | uniq -c
        2 account
    1594 activity
        1 calendar
        1 favourite
        1 planned
        1 products
        1 sleep_result_42645413
        1 sport
        1 strava_routes_42645413
    1022 training
```

Of these I'm only interested in the training files, so I removed the rest.

    λ ls data | wc -l
    1022

I've synced all data since 8 May 2021. So I want the files older than that. Since they filenames contains the dates I can use some AWK commands for scrubbing the files I don't need.

    λ ls data | head -n 1
    training-session-2019-01-18-3142858899-8292b355-fa65-4bf2-88b1-437d78605ec2.json

I don't need the 2022 and 2023 files.

    λ ls | awk -F"-" ' $3==2023' | xargs rm
    λ ls | awk -F"-" ' $3==2022' | xargs rm

And I don't need 2021 after May.

    λ ls | awk -F"-" ' $3==2021 && $4>5' | xargs rm

And no may files after 8 May.

    λ ls | awk -F"-" ' $3==2021 && $4==5 && $5>=8'

Now I have 502 files left

    λ ls | wc -l
    502

## Converting the data to a format Strava accepts

The Strava REST API accepts three formats for training files: GPX which only contains the gps waypoints; TCX which also has pulse data and some meta-data about the activity type; FIT - a Garmin binary format similar to TCX. The json-files from Polar is unfortunately not in any of these formats. A google search found https://www.tredict.com/polarjson2fit-converter/, an online tool that accepts polar json files and returns a zip file containing FIT files. I unpacked the zip files and placed them in a fit/ folder.

## Uploading the files to Strava

I found instructions on how to create an API client on  https://developers.strava.com/docs/getting-started/. 

1. First you register your application and get a client-id and a client-secret.. You then authorize your user by using Oauth2 and receive an authorization code and scope for the user.
2. Then you make a request to exchange an authorization code for an access token.
3. Now, you're ready to upload files

I first tried using the [Python Requests](https://requests.readthedocs.io/en/latest/) library for making these API calls but for some reason my access tokens where not always valid (I now think that was due to me not requesting enough capabilities when register my client).

So I turned to the [Python Stravalib](https://stravalib.readthedocs.io/en/latest/) library instead. For authorizing my app with the user I made this call.

```
from stravalib import Client
import webbrowser

def authorize():
    client = Client()
    url = client.authorization_url(
    	client_id=CLIENT_ID,redirect_uri='http://127.0.0.1:5000/authorization',
        scope=['read', 'read_all', 'profile:read_all', 'profile:write', 		    				   'activity:read','activity:read_all', 'activity:write'])
    webbrowser.open(url)
```

To get an access token you use your client_id, client_secret and the code that is returned as an url parameter to your redirect_uri.  You get a json object back which stravalib encodes as a dict.

```
from stravalib import Client
import pickle

def get_access():
	client = Client()
    access_token = client.exchange_code_for_token(client_id=CLIENT_ID,
            client_secret=CLIENT_SECRET, code=CODE)
    with open('access_token.pickle', 'wb') as f:
        pickle.dump(access_token, f)
```

Now before you start a transaction you have to 

1. Load your access token from disk
2. Check if it's still valid and if not refresh it.

```
import stravalib
import pickle

with open('access_token.pickle', 'rb') as f:
    access_token = pickle.load(f)
if time.time() > access_token['expires_at']:
    refresh_response = client.refresh_access_token(client_id=CLIENT_ID, 
    	client_secret=CLIENT_SECRET,
    	refresh_token=access_token['refresh_token'])
    access_token = refresh_response    
    with open('../access_token.pickle', 'wb') as f:
        pickle.dump(refresh_response, f)
    client.access_token = refresh_response['access_token']
    client.refresh_token = refresh_response['refresh_token']
    client.token_expires_at = refresh_response['expires_at']

```

For actually uploading my Polar data, now encodes as FIT files, I used this snippet. The Strava API uses polling for determining whether an upload has succeeded. I had to set the poll interval to 20s and use 80s as maxtime to get reliable uploads that didn't exceed the ratelimit.

Figuring out which exceptions they upload_activity function could throw required some trial-and-error attempts.

```
from pathlib import Path
import stravalib.exc
import shutil

path = Path('fit')
filenames = [f for f in path.iterdir()]
for filename in filenames:
    with open(filename, 'rb') as f:
        try:
            upload = client.upload_activity(activity_file=f, data_type='fit')
            activity = upload.wait(80, 20)
            if activity:
                shutil.move(filename, 'uploaded/' + filename.name)
                print("new file uploaded: {0}, {1} ({2})".format(activity.name,
                		activity.distance, filename))
        except stravalib.exc.ActivityUploadFailed as e:
            print('Activity upload failed', e)
        except stravalib.exc.ObjectNotFound as e:
            print('Object not found', e)
        except stravalib.exc.RateLimitExceeded as e:
            print('ratelimit', e)
        except stravalib.exc.TimeoutExceeded as e:
            print('Timeout', e)

```

