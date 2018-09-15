#!/usr/bin/python3

import os
import subprocess

BASE64_BINARY = "../build/bin/base64.exe"

def checkResult(name, popen, resultfile):
	print(name)
	(result, stderr) = popen.communicate()
	print("\tReturncode " + str(popen.returncode))
	with open(resultfile, mode="rb") as file:
		expected = file.read()
	if result == expected:
		print("\tResult matches with " + resultfile)
	else:
		with open("_" + resultfile, mode="wb") as file:
			file.write(result)
		print("\tResult does not match with " + resultfile)

def main():
	files = [f for f in os.listdir() if f.startswith("decoded")]
	for decodedfile in files:
		encodedfile = "encoded" + decodedfile[7]
		encoder = subprocess.Popen([BASE64_BINARY, "e", decodedfile, "."], stdout=subprocess.PIPE, stderr=subprocess.PIPE);
		decoder = subprocess.Popen([BASE64_BINARY, "d", encodedfile, "."], stdout=subprocess.PIPE, stderr=subprocess.PIPE);
		checkResult("Encode " + decodedfile, encoder, encodedfile)
		checkResult("Decode " + encodedfile, decoder, decodedfile)

if __name__ == "__main__":
	main()
