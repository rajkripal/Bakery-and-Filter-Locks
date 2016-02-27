import os
reader = open('inp-params.txt', 'r')
n = 0
for line in reader:
  k = line.split(' ')
  n = int(k[0])

files = []
for i in range(n):
  files.append('outputFile' + str(i) + '.txt')

with open('output.txt', 'w') as outfile:
  for fname in files:
    with open(fname) as infile:
      for line in infile:
        outfile.write(line)

for fname in files:
  os.remove(fname)