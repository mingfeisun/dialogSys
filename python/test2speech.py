# coding=utf8

""" This is for testing"""

import sys
from naoqi import ALProxy

ip = "192.168.1.102"
port = 9559

try:
    tts = ALProxy("ALTextToSpeech", ip, port)
except Exception, e:
    print "Error:", e
    sys.exit(1)

testString = "你好，我是人机交互实验室的新成员"

tts.setLanguage("Chinese")
tts.say(testString)
tts.sayToFile(testString, "/home/nao/speechToSay/hello.wav")
