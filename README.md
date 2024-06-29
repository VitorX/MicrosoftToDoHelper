# MicrosoftToDoHelper
This is a repo help creating the Microsoft todo task easily, and this program is still developing.

## Required Packages
    apt install libcurl4-openssl-dev
    apt install libjson-c-dev

## Usage
    Usage: todolist -c {todolistName} -n 500 -t {token} -r 50 -s 2024-6-21
            -c, Create todo List
            -n, Num of todo task of todo list
            -t, Token of used for query or create tasks
            -h, Print help message
            -r, Range of tasks, for example if r is 50, then the tasks 1-50,51-100... will be created. 25 is by default.
            -s, Start date of the first task
            -v, Print REST request response.
            -q, List the todo list owned(not support yet)