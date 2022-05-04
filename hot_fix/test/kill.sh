#!/bin/bash
sudo kill -USR1 `ps -ef|grep main|grep -v grep|awk '{print $2}'`
