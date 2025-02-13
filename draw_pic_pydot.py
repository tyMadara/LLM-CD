import pydot
from PIL import Image

graph = pydot.Dot(graph_type='digraph')

graph.add_node(pydot.Node("Sex"))
graph.add_node(pydot.Node("Age"))
graph.add_node(pydot.Node("CEA"))
graph.add_node(pydot.Node("CA199"))
graph.add_node(pydot.Node("T"))
graph.add_node(pydot.Node("N"))
graph.add_node(pydot.Node("Differentiation"))
graph.add_node(pydot.Node("Diameter"))
graph.add_node(pydot.Node("Progression_Time"))

graph.add_edge(pydot.Edge("CEA", "Differentiation"))
graph.add_edge(pydot.Edge("CA199", "CEA"))
graph.add_edge(pydot.Edge("Diameter", "T",color="#008040"))
graph.add_edge(pydot.Edge("T", "CA199"))
graph.add_edge(pydot.Edge("T", "N"))
graph.add_edge(pydot.Edge("T", "Progression_Time"))
graph.add_edge(pydot.Edge("N", "Differentiation"))
graph.add_edge(pydot.Edge("Differentiation", "Progression_Time",color="#008040"))

graph.write_svg("output/pic/rectum_cancer_ours.svg")

print("**********************************************************************************")

graph = pydot.Dot(graph_type='digraph')

graph.add_node(pydot.Node("Sex"))
graph.add_node(pydot.Node("Age"))
graph.add_node(pydot.Node("CEA"))
graph.add_node(pydot.Node("CA199"))
graph.add_node(pydot.Node("T"))
graph.add_node(pydot.Node("N"))
graph.add_node(pydot.Node("Differentiation"))
graph.add_node(pydot.Node("Diameter"))
graph.add_node(pydot.Node("Progression_Time"))

graph.add_edge(pydot.Edge("CA199", "CEA"))
graph.add_edge(pydot.Edge("Progression_Time", "Differentiation", color="#8B0000"))
graph.add_edge(pydot.Edge("T", "Progression_Time"))


graph.write_svg("output/pic/rectum_cancer_baseline.svg")

print("**********************************************************************************")

graph = pydot.Dot(graph_type='digraph')

graph.add_node(pydot.Node("Age"))
graph.add_node(pydot.Node("PreRadiotherapy_NRS2002"))
graph.add_node(pydot.Node("PreRadiotherapy_Albumin"))
graph.add_node(pydot.Node("PostRadiotherapy_Albumin"))
graph.add_node(pydot.Node("Skin_Mucosa_Status"))
graph.add_node(pydot.Node("Oral_Mucosa_Status"))
graph.add_node(pydot.Node("Sex"))
graph.add_node(pydot.Node("Smoking_Status"))
graph.add_node(pydot.Node("Drinking_Status"))
graph.add_node(pydot.Node("KPS_Score"))
graph.add_node(pydot.Node("T"))
graph.add_node(pydot.Node("N"))
graph.add_node(pydot.Node("Clinical_Stage"))
graph.add_node(pydot.Node("Gastrointestinal_Reaction"))


graph.add_edge(pydot.Edge("Age", "KPS_Score"))
graph.add_edge(pydot.Edge("PreRadiotherapy_NRS2002", "PreRadiotherapy_Albumin"))
graph.add_edge(pydot.Edge("PreRadiotherapy_Albumin", "PostRadiotherapy_Albumin", color="#008040"))
graph.add_edge(pydot.Edge("Skin_Mucosa_Status", "Oral_Mucosa_Status"))
graph.add_edge(pydot.Edge("Skin_Mucosa_Status", "Gastrointestinal_Reaction", color="#008040"))
graph.add_edge(pydot.Edge("Sex", "Smoking_Status"))
graph.add_edge(pydot.Edge("Smoking_Status", "Drinking_Status"))
graph.add_edge(pydot.Edge("Drinking_Status", "KPS_Score"))
graph.add_edge(pydot.Edge("Drinking_Status", "Skin_Mucosa_Status", color="#008040"))
graph.add_edge(pydot.Edge("KPS_Score", "PreRadiotherapy_NRS2002"))
graph.add_edge(pydot.Edge("KPS_Score", "PreRadiotherapy_Albumin"))
graph.add_edge(pydot.Edge("KPS_Score", "T"))
graph.add_edge(pydot.Edge("T", "Clinical_Stage", color="#008040"))
graph.add_edge(pydot.Edge("Clinical_Stage", "N"))


graph.write_svg("output/pic/head_neck_ours.svg")

print("**********************************************************************************")

graph = pydot.Dot(graph_type='digraph')

graph.add_node(pydot.Node("Age"))
graph.add_node(pydot.Node("PreRadiotherapy_NRS2002"))
graph.add_node(pydot.Node("PreRadiotherapy_Albumin"))
graph.add_node(pydot.Node("PostRadiotherapy_Albumin"))
graph.add_node(pydot.Node("Skin_Mucosa_Status"))
graph.add_node(pydot.Node("Oral_Mucosa_Status"))
graph.add_node(pydot.Node("Sex"))
graph.add_node(pydot.Node("Smoking_Status"))
graph.add_node(pydot.Node("Drinking_Status"))
graph.add_node(pydot.Node("KPS_Score"))
graph.add_node(pydot.Node("T"))
graph.add_node(pydot.Node("N"))
graph.add_node(pydot.Node("Clinical_Stage"))
graph.add_node(pydot.Node("Gastrointestinal_Reaction"))

graph.add_edge(pydot.Edge("Skin_Mucosa_Status", "Oral_Mucosa_Status"))
graph.add_edge(pydot.Edge("Sex", "Smoking_Status"))
graph.add_edge(pydot.Edge("Smoking_Status", "Drinking_Status"))
graph.add_edge(pydot.Edge("Clinical_Stage", "T", color="#8B0000"))
graph.add_edge(pydot.Edge("Clinical_Stage", "N"))
graph.add_edge(pydot.Edge("KPS_Score", "PreRadiotherapy_Albumin"))

graph.write_svg("output/pic/head_neck_baseline.svg")

