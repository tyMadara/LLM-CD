import argparse
import glob

parser = argparse.ArgumentParser(description="",
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("instance", help="")
args = parser.parse_args()

mapFile = open("mappings/" + args.instance + ".mapping")
varNames = mapFile.read().strip().split("\n")


def translateFile(file):
	baseName = file.split("/")[-1]
	outFile = open("experiment2-constraints/" + args.instance + "-CaMML/" + baseName, "w")
	outFile.write("arcs {\n")
	inFile = open(file)

	nDirected = int(inFile.readline())

	for i in range(nDirected):
		a, b = map(int, inFile.readline().strip().split(" "))
		outFile.write("\t%s -> %s 1.0; \n" %(varNames[a], varNames[b]))


	nUndirected = int(inFile.readline())

	for i in range(nUndirected):
		a, b = map(int, inFile.readline().strip().split(" "))
		outFile.write("\t%s -- %s 1.0; \n" %(varNames[a], varNames[b]))

	nAbsence = int(inFile.readline())
	assert(nAbsence == 0)

	nOrd = int(inFile.readline())

	for i in range(nOrd):
		a, b = map(int, inFile.readline().strip().split(" "))
		outFile.write("\t%s >> %s 1.0; \n" %(varNames[b], varNames[a]))

	nAncestral = int(inFile.readline())

	for i in range(nAncestral):
		a, b = map(int, inFile.readline().strip().split(" "))
		outFile.write("\t%s => %s 0.9999; \n" %(varNames[a], varNames[b]))

	outFile.write("}\n")
	outFile.close()



for file in glob.glob("experiment2-constraints/" + args.instance + "/*"):
	translateFile(file)