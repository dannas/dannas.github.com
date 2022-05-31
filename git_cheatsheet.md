# Concepts
blob
tree
commit
refs
remote refs
tag
wc
staging area   
dag
remotes
branch
HEAD
tag

# Plumbing
git cat-file
git ls-files

git init
git commit
git branch MY_BRANCH
git checkout MY_BRANCH
git fetch
git pull --rebase ORIGIN MY_BRANCH
git push
git push --force-with-lease

git add -i
git rebase -i
git rebase FIRST_THIS THEN_THIS             Rewrite THEN_THIS csets on top of FIRST_THIS
git rebase -X renormalize main              Ignorera radslutsskillnader
git log --graph --pretty=oneline --abbrev-commit --all           Or use gitk
git reset SHA1
git switch

TODO(dannas): Clarify ditinction between path and branch parameters to restore/checkout/reset
https://redfin.engineering/two-commits-that-wrecked-the-user-experience-of-git-f0075b77eab1
git restore                                 Restore files in wc from index or commit
git restore --staged foo.c


git reset                                   Moves current branch ref
git checkout                                Moves HEAD
git switch

man gitrevisions
man gittutorial
man gittutorial2

http://sethrobertson.github.io/GitBestPractices/
http://sethrobertson.github.io/GitFixUm/fixup.html

git revert          makes new commit taht reverts the changes made by other commits
git restore         restore files in wc from index or another commit.
                    Or restore files in index from other commit
                    Does not update your branch
git reset           Update your branch, move tip in order to add or remove commits
git rebase upstream my-branch --onto new_base
                    Move my-branch from upstream to new_base

# Git tags
git tag             list all tags
git tag -a TAGNAME -m "commit msg" # Create annotated tag (the type we want)
git push --follow-tags # Pushes all reachable annotated tags

# Git revisions
sha1
output from git describe
refname
@                  shortcut for HEAD
refname@date
refname@n
^n                 n'th parent
~n                 n't first parent 

# Git revision range
git log single revision        means set of commits reachable from the given commit
r1..r2 (two dot)               commits reachable from r2, excluding those reachable from r1 (r1=from, r2=to)
r1...r2 (three dot)            commits reachable from r2 and r1 but not from both
