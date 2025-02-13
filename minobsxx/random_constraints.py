from random import *
n = 8

vars = ["HISTORY", "CVP", "PCWP", "HYPOVOLEMIA", "LVEDVOLUME", "LVFAILURE",
        "STROKEVOLUME", "ERRLOWOUTPUT", "HRBP", "HREKG", "ERRCAUTER", "HRSAT",
        "INSUFFANESTH", "ANAPHYLAXIS", "TPR", "EXPCO2", "KINKEDTUBE", "MINVOL",
        "FIO2", "PVSAT", "SAO2", "PAP", "PULMEMBOLUS", "SHUNT", "INTUBATION",
        "PRESS", "DISCONNECT", "MINVOLSET", "VENTMACH", "VENTTUBE", "VENTLUNG",
        "VENTALV", "ARTCO2", "CATECHOL", "HR", "CO", "BP"]

while True:
    input()
    a = randint(0, n-1)
    b = randint(0, n-1)
    print(vars[a], "----->", vars[b], "\t(", a, "----->", b, ")")


