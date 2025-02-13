n = 48
instance = "barley"
dataSize = "8000"
ancestralModelFile = open("bdeu/"+instance + "_" + dataSize +"_ancestral_results")
allModelFile = open("bdeu/"+instance + "_" + dataSize +"_all_results")


modelStringCache = dict()
modelStringCache["asia"] = "[asia][tub|asia][smoke][lung|smoke][bronc|smoke][either|lung:tub][xray|either][dysp|bronc:either]"
modelStringCache["alarm"] = "[HYPOVOLEMIA][LVFAILURE][ERRLOWOUTPUT][ERRCAUTER][INSUFFANESTH][ANAPHYLAXIS][KINKEDTUBE][FIO2][PULMEMBOLUS][INTUBATION][DISCONNECT][MINVOLSET][HISTORY|LVFAILURE][LVEDVOLUME|HYPOVOLEMIA:LVFAILURE][STROKEVOLUME|HYPOVOLEMIA:LVFAILURE][TPR|ANAPHYLAXIS][PAP|PULMEMBOLUS][SHUNT|INTUBATION:PULMEMBOLUS][VENTMACH|MINVOLSET][CVP|LVEDVOLUME][PCWP|LVEDVOLUME][VENTTUBE|DISCONNECT:VENTMACH][PRESS|INTUBATION:KINKEDTUBE:VENTTUBE][VENTLUNG|INTUBATION:KINKEDTUBE:VENTTUBE][MINVOL|INTUBATION:VENTLUNG][VENTALV|INTUBATION:VENTLUNG][PVSAT|FIO2:VENTALV][ARTCO2|VENTALV][EXPCO2|ARTCO2:VENTLUNG][SAO2|PVSAT:SHUNT][CATECHOL|ARTCO2:INSUFFANESTH:SAO2:TPR][HR|CATECHOL][HRBP|ERRLOWOUTPUT:HR][HREKG|ERRCAUTER:HR][HRSAT|ERRCAUTER:HR][CO|HR:STROKEVOLUME][BP|CO:TPR]"
modelStringCache["child"] = "[BirthAsphyxia][Disease|BirthAsphyxia][LVH|Disease][DuctFlow|Disease][CardiacMixing|Disease][LungParench|Disease][LungFlow|Disease][Sick|Disease][HypDistrib|DuctFlow:CardiacMixing][HypoxiaInO2|CardiacMixing:LungParench][CO2|LungParench][ChestXray|LungParench:LungFlow][Grunting|LungParench:Sick][LVHreport|LVH][Age|Disease:Sick][LowerBodyO2|HypDistrib:HypoxiaInO2][RUQO2|HypoxiaInO2][CO2Report|CO2][XrayReport|ChestXray][GruntingReport|Grunting]"
modelStringCache["sachs"] = "[PKC][Plcg][PIP3|Plcg][PKA|PKC][Jnk|PKA:PKC][P38|PKA:PKC][PIP2|PIP3:Plcg][Raf|PKA:PKC][Mek|PKA:PKC:Raf][Erk|Mek:PKA][Akt|Erk:PKA]"
modelStringCache["insurance"] = "[Age][Mileage][SocioEcon|Age][GoodStudent|SocioEcon:Age][RiskAversion|Age:SocioEcon][OtherCar|SocioEcon][VehicleYear|SocioEcon:RiskAversion][MakeModel|SocioEcon:RiskAversion][SeniorTrain|Age:RiskAversion][HomeBase|RiskAversion:SocioEcon][AntiTheft|RiskAversion:SocioEcon][RuggedAuto|MakeModel:VehicleYear][Antilock|MakeModel:VehicleYear][DrivingSkill|Age:SeniorTrain][CarValue|MakeModel:VehicleYear:Mileage][Airbag|MakeModel:VehicleYear][DrivQuality|DrivingSkill:RiskAversion][Theft|AntiTheft:HomeBase:CarValue][Cushioning|RuggedAuto:Airbag][DrivHist|DrivingSkill:RiskAversion][Accident|Antilock:Mileage:DrivQuality][ThisCarDam|Accident:RuggedAuto][OtherCarCost|Accident:RuggedAuto][MedCost|Accident:Age:Cushioning][ILiCost|Accident][ThisCarCost|ThisCarDam:CarValue:Theft][PropCost|OtherCarCost:ThisCarCost]"
modelStringCache["water"] = "[C_NI_12_00][CKNI_12_00][CBODD_12_00][CKND_12_00][CNOD_12_00][CBODN_12_00][CKNN_12_00][CNON_12_00][C_NI_12_15|C_NI_12_00][CKNI_12_15|CKNI_12_00][CBODD_12_15|C_NI_12_00:CKNI_12_00:CBODD_12_00:CNOD_12_00:CBODN_12_00][CKND_12_15|CKNI_12_00:CKND_12_00:CKNN_12_00][CNOD_12_15|CBODD_12_00:CNOD_12_00:CNON_12_00][CBODN_12_15|CBODD_12_00:CBODN_12_00:CNON_12_00][CKNN_12_15|CKND_12_00:CKNN_12_00][CNON_12_15|CNOD_12_00:CBODN_12_00:CKNN_12_00:CNON_12_00][C_NI_12_30|C_NI_12_15][CKNI_12_30|CKNI_12_15][CBODD_12_30|C_NI_12_15:CKNI_12_15:CBODD_12_15:CNOD_12_15:CBODN_12_15][CKND_12_30|CKNI_12_15:CKND_12_15:CKNN_12_15][CNOD_12_30|CBODD_12_15:CNOD_12_15:CNON_12_15][CBODN_12_30|CBODD_12_15:CBODN_12_15:CNON_12_15][CKNN_12_30|CKND_12_15:CKNN_12_15][CNON_12_30|CNOD_12_15:CBODN_12_15:CKNN_12_15:CNON_12_15][C_NI_12_45|C_NI_12_30][CKNI_12_45|CKNI_12_30][CBODD_12_45|C_NI_12_30:CKNI_12_30:CBODD_12_30:CNOD_12_30:CBODN_12_30][CKND_12_45|CKNI_12_30:CKND_12_30:CKNN_12_30][CNOD_12_45|CBODD_12_30:CNOD_12_30:CNON_12_30][CBODN_12_45|CBODD_12_30:CBODN_12_30:CNON_12_30][CKNN_12_45|CKND_12_30:CKNN_12_30][CNON_12_45|CNOD_12_30:CBODN_12_30:CKNN_12_30:CNON_12_30]"
modelStringCache["barley"] = "[jordtype][komm][forfrugt][pesticid][saatid][sort][nplac][saamng][tkvs][partigerm][nedbarea|komm][aar_mod|komm:jordtype][potnmin|jordtype:forfrugt][exptgens|jordtype:forfrugt:pesticid][rokap|jordtype][srtprot|sort][dg25|saatid][saakern|saamng:tkvs][frspdag|saatid][sorttkv|sort][srtsize|sort][nmin|jordtype:nedbarea][nopt|exptgens:pesticid][dgv1059|saatid:rokap][jordinf|frspdag][dgv5980|rokap][jordn|nmin:aar_mod:potnmin][mod_nmin|nmin:aar_mod][markgrm|partigerm:jordinf][bgbyg|dgv1059:dgv5980][ngodnt|forfrugt:exptgens:mod_nmin][ngodnn|nopt:jordn][antplnt|saakern:markgrm][ngodn|ngodnt:ngodnn][nprot|jordn:ngodn][ngtilg|ngodn:nplac:dg25][ntilg|ngtilg:jordn][aks_m2|antplnt:ntilg:dgv1059:sorttkv][keraks|ntilg:dgv1059:aks_m2][aks_vgt|ntilg:dgv5980:aks_m2][ksort|keraks:aks_vgt:srtsize][udb|aks_m2:aks_vgt][tkv|aks_m2:keraks:ntilg:sorttkv][slt22|keraks:aks_vgt:srtsize][s2225|keraks:aks_vgt:srtsize][s2528|keraks:aks_vgt:srtsize][protein|nprot:dgv1059:srtprot:ksort][spndx|ntilg:dgv5980:ksort]"
modelStringCache["cancer"] = "[Pollution][Smoker][Cancer|Pollution:Smoker][Xray|Cancer][Dyspnoea|Cancer]"
modelStringCache["earthquake"] = "[Burglary][Earthquake][Alarm|Burglary:Earthquake][JohnCalls|Alarm][MaryCalls|Alarm]"
modelStringCache["survey"] = "[A][S][E|A:S][O|E][R|E][T|O:R]"
modelStringCache["mildew"] = "[straaling_1][temp_1][meldug_1][lai_0][straaling_2][temp_2][straaling_3][temp_3][straaling_4][temp_4][middel_1][middel_2][middel_3][nedboer_1][nedboer_2][nedboer_3][lai_1|lai_0:meldug_1][foto_1|lai_1:temp_1:straaling_1][mikro_1|lai_1:temp_1:nedboer_1][dm_1|foto_1][meldug_2|middel_1:mikro_1:meldug_1][lai_2|lai_1:meldug_2][foto_2|lai_2:temp_2:straaling_2][mikro_2|lai_2:temp_2:nedboer_2][dm_2|foto_2:dm_1][meldug_3|middel_2:mikro_2:meldug_2][lai_3|lai_2:meldug_3][foto_3|lai_3:temp_3:straaling_3][mikro_3|lai_3:temp_3:nedboer_3][dm_3|foto_3:dm_2][meldug_4|middel_3:mikro_3:meldug_3][lai_4|lai_3:meldug_4][foto_4|lai_4:temp_4:straaling_4][dm_4|foto_4:dm_3][udbytte|dm_4]"



addedTimes = dict()

addedTimes["cancer_250"] = 0.01
addedTimes["cancer_1000"] = 0.0133333333
addedTimes["earthquake_250"] = 0.0133333333
addedTimes["earthquake_1000"] = 0.01
addedTimes["survey_250"] = 0.015
addedTimes["survey_1000"] = 0.01166666
addedTimes["asia_250"] = 0.015
addedTimes["asia_1000"] = 0.015
addedTimes["sachs_250"] = 0.02
addedTimes["sachs_1000"] = 0.0266666
addedTimes["child_500"] = 0.14333333
addedTimes["child_2000"] = 0.156666666
addedTimes["insurance_500"] = 0.4983333333
addedTimes["insurance_2000"] = 0.555
addedTimes["water_1000"] = 1.116666666
addedTimes["water_4000"] = 1.166666666
addedTimes["mildew_8000"] = 22.08666666666
addedTimes["mildew_32000"] = 32.508333333333
addedTimes["alarm_1000"] = 2.15
addedTimes["alarm_4000"] = 2.203333333333
addedTimes["barley_2000"] = 18.49
addedTimes["barley_8000"] = 31.2983333333333

bestScores = dict()

bestScores["cancer_250"] = 525.3
bestScores["cancer_1000"] = 2107.7
bestScores["earthquake_250"] = 131.8
bestScores["earthquake_1000"] = 457.6
bestScores["survey_250"] = 1014.7
bestScores["survey_1000"] = 3977.6
bestScores["asia_250"] = 598.1
bestScores["asia_1000"] = 2307.5
bestScores["sachs_250"] = 2002.6
bestScores["sachs_1000"] = 7541.7
bestScores["child_500"] = 6619.0
bestScores["child_2000"] = 25222.2
bestScores["insurance_500"] = 7263.3
bestScores["insurance_2000"] = 27286.0
bestScores["water_1000"] = 13306.1
bestScores["water_4000"] = 51941.7
bestScores["mildew_8000"] = 366734.7
bestScores["mildew_32000"] = 1354857.3
bestScores["alarm_1000"] = 11105.6
bestScores["alarm_4000"] = 42751.1
bestScores["barley_2000"] = 110098.1
bestScores["barley_8000"] = 409719.4

class Parser:
	def __init__(self, n, instance, dataSize, modelFile):
		self.n = n
		self.instance = instance
		self.dataSize = dataSize
		self.modelFile = modelFile
		self.mapFile = open("mappings/" + instance + ".mapping")
		self.mapping = dict()
		self.rmapping = dict()


		self.scoreTotals = [0 for i in range(1000)]
		self.shdTotals = [0 for i in range(1000)]
		self.missingTotals = [0 for i in range(1000)]
		self.extraTotals = [0 for i in range(1000)]
		self.reversedTotals = [0 for i in range(1000)]
		self.tmTotals = [0 for i in range(1000)]
		self.counts = [0 for i in range(1000)]
		self.models = [[] for i in range(1000)]

		for i in range(n):
			var = self.mapFile.readline().strip()
			self.rmapping[i] = var
			self.mapping[var] = i

	def model2network(self, model):
		network = [[0]*n for j in range(n)]

		for i in range(n):
			varStr = model[1:model.find("]")]
			model = model[model.find("]") + 1:]


			
			if (varStr.find("|") == -1):
				#print("Parents of var ", mapping[varStr], ": ", end="")
				#print("")
				continue

			else:
				var, rest = varStr.split("|")
				pars = rest.split(":")

				intPars = []

				#print("Parents of var ", mapping[var], ": ", end="")
				for par in pars:
					intPars.append(self.mapping[par])

					network[self.mapping[par]][self.mapping[var]] = 1
				intPars.sort()
				#print(" ".join(map(str, intPars)))

		return network


	def hammingDAG(self, trueBN, learnedBN):
		numMissing = 0
		numExtra = 0
		numReversed = 0

		for i in range(n):
			for j in range(n):
				if learnedBN[i][j] == 1 and trueBN[i][j] == 0:
					# Check first for if the reverse arc is present.
					if trueBN[j][i] == 1:
						numReversed += 1
					else:
						numExtra += 1

				elif learnedBN[i][j] == 0 and trueBN[i][j] == 1:
					# Avoid double counting the number of reversed arcs
					if learnedBN[j][i] != 1:
						numMissing += 1

		return (numMissing + numExtra + numReversed, numMissing, numExtra, numReversed)


	def parse(self):
		lastSz = -1
		trueBN = self.model2network(modelStringCache[instance])
		parsedStr = ""

		while True:
			line1 = self.modelFile.readline()

			if (not line1):
				break

			if (line1.strip() == "" or line1[0] == "#"):
				continue



			model = self.modelFile.readline()
			line3 = self.modelFile.readline()
			line4 = self.modelFile.readline()

			size = int(line1)
			score = int(line3)
			tm = sum(map(float, line4.strip().split(",")))

			self.scoreTotals[size] += score


			if (size != lastSz):
				lastSz = size
		 
			info = self.hammingDAG(trueBN, self.model2network(model))
			self.shdTotals[size] +=  info[0]
			self.missingTotals[size] += info [1]
			self.extraTotals[size] += info [2]
			self.reversedTotals[size] += info [3]
			self.tmTotals[size] += tm
			self.counts[size] += 1
			self.models[size].append(model)

		parsedFile = open(instance + "_results_parsed", "w")
		for i in range(1000):
			if self.counts[i] != 0:
				assert(self.counts[i] == 6 or self.counts[i] == 30)
				parsedStr += str.format("%.1f %.1f %.1f %.1f\\%%\n" %(self.missingTotals[i]/self.counts[i], self.extraTotals[i]/self.counts[i], self.reversedTotals[i]/self.counts[i], (self.scoreTotals[i]/self.counts[i]/1000000 - bestScores[instance + "_" + dataSize]) / bestScores[instance + "_" + dataSize]  * 100))
				print("Size: %d \t Avg Score: %f \t Avg SHD: %f \t Avg Missing: %f \t Avg Extra: %f \t Avg Reversed: %f \t Samples: %d \t t: %f\n" %(i, (self.scoreTotals[i]/self.counts[i]/1000000 - bestScores[instance + "_" + dataSize]) / bestScores[instance + "_" + dataSize] , self.shdTotals[i]/self.counts[i], self.missingTotals[i]/self.counts[i], self.extraTotals[i]/self.counts[i], self.reversedTotals[i]/self.counts[i], self.counts[i], self.tmTotals[i]/self.counts[i] + addedTimes[instance + "_" + dataSize]))

				parsedFile.write(str(i) + "\n")
				for model in self.models[i]:
					parsedFile.write("\"" + model.strip() + "\",\n")
		parsedFile.close()


		return parsedStr

parserAncestral = Parser(n, instance, dataSize, ancestralModelFile)
parserAll = Parser(n, instance, dataSize, allModelFile)

str1 = parserAncestral.parse()
print("------------------------------------------------------")
str2 = parserAll.parse()


def formatResults(s1, s2):
	lines1 = s1.strip().split("\n")
	lines2 = s2.strip().split("\n")

	print("& $0^*$ & ", end="")
	ls = lines1.pop(0).split(" ")

	print(ls[0], "&", ls[1], "&", ls[2], "& &", ls[3], "\\\\")

	for i in range(4):
		if (i == 0):
			print("& & 10 / 5 & ", end="")
		elif (i == 1):
			print("& & 25 / 10 & ", end="")
		elif (i == 2):
			print("& & 50 / 15 & ", end="")
		else:
			print("& & 100 / 20 & ", end="")

		ls1 = lines1[i].split(" ")
		ls2 = lines2[i].split(" ")

		print(ls1[0], "/", ls2[0], "&", ls1[1], "/", ls2[1], "&", ls1[2], "/", ls2[2], "& &", ls1[3], "/", ls2[3], "\\\\")

formatResults(str1, str2)

