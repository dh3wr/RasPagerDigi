#!/bin/bash
cd /home/pi/RasPagerDigi/software/raspager-java
sudo java -jar  -classpath jna-4.1.0.jar:. -jar raspager.jar -configfile=/home/pi/RasPagerDigi/software/raspager-java/db0eif.cfg -v
