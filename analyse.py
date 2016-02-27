import os
reader = open('output.txt', 'r')
count = 0
request = []
enter = []
new_count = 0

for line in reader:
  count += 1
  if count%3 == 0:
    continue
  new_count += 1
  a = line.split(' ')
  b = a[4].split(':')
  time = int(b[0]) * 3600 + int(b[1]) * 60 + int(b[2])
  if new_count % 2 == 1:
    request.append(time)
  else:
    enter.append(time)

avgtimetocs = (sum(enter) - sum(request)) / float(len(enter))
avgtimetocs = round(avgtimetocs, 2)
print 'average time to cs is ' + str(avgtimetocs) + 'seconds'
os.remove('output.txt')
