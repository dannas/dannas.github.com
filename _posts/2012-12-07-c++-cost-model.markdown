---
layout: post
title: C++ cost model
---

I've [previously][c-cost] done experiments on a C cost model. Those are valid
for C++ too, but there are plenty of more to examine: STL data types;
virtual functions; the C++ way of doing memory handling etcetera.

[c-cost]: /2012/12/03/c-cost-model.html

String operations (n=2000)

    s += 'a'                              24
    s += "0123456789"                     88
    string()                              20
    string(scopy)                         20
    k = s[i]                              12
    ++sit                                 10
    sit++                                 18
    k = *sit; ++sit                       14
    s == "0123456789"                     79

Stringstream operations (n=2000)

    ss << "abc"                           93
    ss << "0123456789"                   121
    ss << fi                            1522
    ss << i                              177

Vector operations (n=2000)

    vector<int>()                        108
    v.push_back(i)                        36
    k = v[i]                               8
    ++vit                                  8
    vit++                                 18
    k = *vit; ++vit                       15

Parameter passing (n=2000)

    byval(scopy, scopy)                   64
    byref(scopy, scopy)                    7
    inlswap(v, i, j)                      38
    swap(v, i, j)                         38

Input (n=2000)

    cin >> s                              56
    cin >> d                              44
    cin >> f                              54

Output (n=2000)

    fout << "abcdef\n"                   111
    fout << "abcdef" << endl             944
    fout << "abcdef" << 3.14 << 42      1604

Memory allocation (n=2000)

    delete new char                       91
    delete[] new char[16]                108
    delete[] new char[1000]              144
    delete[] new char[2000]              148

