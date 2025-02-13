from random import *
from math import *
import argparse

n = 37
file = open("alarm.net", "r")

parser = argparse.ArgumentParser(description="Input the percentage of constraints you want to generate",
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("percentage", help="Enter a decimal number")
args = parser.parse_args()

p = float(args.percentage)


mapping = dict()
rmapping = dict()
graph = [ [] for i in range(n) ]
adj = [ [0]*n for i in range(n) ]

ancestralConstraints = []
absenceConstraints = []
existConstraints = []
orderingConstraints = []

topological = []



for i in range(n): 
	s = file.readline()
	s = s[5:]
	s = s.strip()

	#print("Mapping %s to %d" %(s, i))
	mapping[s] = i
	rmapping[i] = s

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
	p

	for each in words[1:]:
		graph[node].append(mapping[each])
		adj[mapping[each]][node] = 1 
		existConstraints.append( (mapping[each], node) )

	assert(node == i)


	file.readline()
	file.readline()
	file.readline()


for i in range(n):
	for j in range(n):
		if (i == j):
			continue
		if (adj[i][j] == 0):
			absenceConstraints.append((i, j))




####################################################################################
# Compute all ancestral constraints
visited = set()


def ancestors(x, start):
	if x in visited:
		return
	visited.add(x)

	if (start != x):
		ancestralConstraints.append((x, start))
		#print("Constraint: %s ---> %s" %(rmapping[x], rmapping[start]))

	for parent in graph[x]:
		ancestors(parent, start)


for i in range(n):
	visited = set()
	ancestors(i, i)

#print("Total number of positive constraints: " + str(len(allConstraints)))
ancestralConstraints = list(set(ancestralConstraints))




####################################################################################
# Ordering constraints:
# Use DFS to get an arbitrary topological ordering of the nodes
# Output some proportion of the (n choose 2) possible pairwise ordering constraints
marked = set()

def dfs(x): 
	if x in marked:
		return
	for y in range(n):
		if adj[x][y]:
			dfs(y)

	marked.add(x)
	topological.append(x)


for i in range(n):
	if i not in marked:
		dfs(i)

topological = list(reversed(topological))

# Randomly sample (n choose 2) * p constraints


assert(len(topological) == n)

for i in range(n-1):
	for j in range(i+1, n):
		orderingConstraints.append((topological[i], topological[j]))

####################################################################################

shuffle(ancestralConstraints)
shuffle(existConstraints)
shuffle(orderingConstraints)
shuffle(absenceConstraints)


directed = []
undirected = []

for i in range(int(ceil(len(existConstraints) * p))):
	if (randint(0,1)==0):
		directed.append(existConstraints[i])
	else:
		undirected.append(existConstraints[i])


nAbs = int(ceil(len(absenceConstraints) * p))
nAnc = int(ceil(len(ancestralConstraints) * p))
nOrd = int(ceil(len(orderingConstraints) * p))

assert(len(orderingConstraints) == n * (n-1) / 2)




print(len(directed))

for i in range(len(directed)):
	print("%d %d"%(directed[i][0], directed[i][1]))
	#print(rmapping[directed[i][0]], rmapping[directed[i][1]])

print(len(undirected))

for i in range(len(undirected)):
	if (randint(0,1) == 0):
		print("%d %d"%(undirected[i][1], undirected[i][0]))
	else:
		print("%d %d"%(undirected[i][0], undirected[i][1]))
	#print(rmapping[undirected[i][0]], rmapping[undirected[i][1]])



print("0")

#print(nAbs)

# for i in range(nAbs):
# 	print("%d %d"%(absenceConstraints[i][0], absenceConstraints[i][1]))
# 	#print(rmapping[absenceConstraints[i][0]], rmapping[absenceConstraints[i][1]])

print(nOrd)

for i in range(nOrd):
	print("%d %d"%(orderingConstraints[i][0], orderingConstraints[i][1]))


print(nAnc)

for i in range(nAnc):
	print("%d %d"%(ancestralConstraints[i][0], ancestralConstraints[i][1]))
	#print(rmapping[ancestralConstraints[i][0]], rmapping[ancestralConstraints[i][1]])