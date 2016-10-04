curl -XPOST 'https://api.wit.ai/speech?v=20160526' -i -L -H "Authorization: Bearer W36NPJ4UZ2RFE2Q5UUQQNWJL62BRITGA" -H "Content-Type: audio/raw;encoding=signed-integer;bits=16;rate=16k;endian=little" --data-binary "@raw.dat"

# curl -XPOST 'https://api.wit.ai/speech?v=20160526' -i -L -H "Authorization: Bearer W36NPJ4UZ2RFE2Q5UUQQNWJL62BRITGA" -H "Content-Type: audio/wav" --data-binary "@test.wav"
