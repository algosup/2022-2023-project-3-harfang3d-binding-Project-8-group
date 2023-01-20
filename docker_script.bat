@echo off

SET absolute=%~dp0

docker build -t fabgen %absolute%
docker run -v %absolute%:/app -it fabgen