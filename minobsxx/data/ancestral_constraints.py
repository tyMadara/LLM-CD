from random import *
from math import *

n = 35
p = 1
mapping = dict()
rmapping = dict()
graph = [ [] for i in range(n) ]


file = open("mildew.net", "r")

for i in range(n): 
	s = file.readline()
	s = s[5:]
	s = s.strip()

	#print("Mapping %s to %d" %(s, i))
	mapping[s] = i;
	rmapping[i] = s;

	file.readline()
	file.readline()
	file.readline()

for i in range(n):
	s = file.readline()
	s = s[12:] 
	s = s[: -3]
	s = s.strip()

	words = s.split(" ")
	if len(words) > 1:
		del words[1]

	node = mapping[words[0]]

	for each in words[1:]:
		graph[node].append(mapping[each])

	assert(node == i)


	file.readline()
	file.readline()
	file.readline()


#print(graph)

allConstraints = set()
visited = set()

def ancestors(x, start):
	if x in visited:
		return
	visited.add(x)

	if (start != x):
		allConstraints.add((x, start))
		#print("Constraint: %s ---> %s" %(rmapping[x], rmapping[start]))

	for parent in graph[x]:
		ancestors(parent, start)


for i in range(n):
	visited = set()
	ancestors(i, i)


#print("Total number of positive constraints: " + str(len(allConstraints)))

constraints = []

for each in allConstraints:
	constraints.append(each)

shuffle(constraints)


n0 = int(ceil(len(allConstraints) * p))

print(n0)

for i in range(n0):
	print("%d %d"%(constraints[i][0], constraints[i][1]))

# for each in allConstraints:
# 	print("%d %d"%(each[0], each[1]))