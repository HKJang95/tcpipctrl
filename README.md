# TCP IP Control assignment
## Abstract
#### This is the project that I've done on 2020-09 for IPP program at Hyvision System.
## Environment
### Camera
#### Crevis MQ-80G (Mono8 / Rolling shutter)
### Light
#### LLight 4 Ch. controller
#### LLight 12V white light
## How it works
#### Host : Inits light and cam
#### Host : Wait for comamnd
#### Client : Sends command
#### Host : Command execution
#### Host : Respond to Client (Error on complete)
#### :: For detail, please refer enclosed pptx file.
## Current Issue
#### If client is multiple, Host cannot send response properly, and client stalls.
