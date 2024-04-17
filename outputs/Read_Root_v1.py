import ROOT
import numpy as np
import matplotlib.pyplot as plt
import code

def read_tree(tree):
    # Check if the tree is successfully loaded
    if tree:
        # Get the total number of entries in the tree
        numEntries = tree.GetEntries()

        # Get the list of branches in the tree
        branches = tree.GetListOfBranches()

        # Get the total number of branches
        numBranches = len(branches)

        # Create a dictionary to map branch indices to their names
        branchIndex_map = {}
        branchIndx_map = {}
        for i, branch in enumerate(branches):
            branch_name = branch.GetName()
            branchIndex_map[branch_name] = i
            branchIndx_map[i] = branch_name

        # Create an empty NumPy array to store the data
        data_array = []

        # Loop through each event in the tree
        for i, event in enumerate(tree):
            if(i%100000 == 0):
                print(f"Read Entry {i}")
            data_array.append([])
            # Loop through each branch in the tree
            for j in range(numBranches):
                # Get the value of the branch for the current event and store it in the data array
                data_array[i].append(getattr(event, f"{branchIndx_map[j]}"))
        
        return branchIndx_map, branchIndex_map, data_array, numEntries
    else:
        # If the tree is not loaded, return None
        print("Check the Tree name")
        return None, 0


def read_file(fname):
    # Open the file to read
    root_file = ROOT.TFile.Open(fname)

    # Get the list of keys
    keys = root_file.GetListOfKeys()

    # Create a dictionary storing the different Tree names and corresponding data
    fTrees = {}

    #check if the key is for TTree object and read the data
    for key in keys:
        # Get the object associated with the key
        obj = root_file.Get(key.GetName())  

        # Check if the object is a TTree
        if isinstance(obj, ROOT.TTree):
            fTree_name = key.GetName()                                                      # Get the TTree name
            branchIndx_map, branchIndex_map, data_array, numEntries = read_tree(obj)        # Read the TTree
            fTrees[fTree_name] = (numEntries, branchIndx_map, branchIndex_map, data_array)                  # Store the output in a dictionary

    return fTrees

##################################################################################################################
##                                             MAIN PROGRAM                                                     ##
##################################################################################################################

# Variable containing the required data
numParticleInSensVol = []

##currEnergy = minEnergy
##while(currEnergy <= maxEnergy):                                             # Loop over the energies
##fname = "output_SheildingPhysics.root"
##fname = "output_QGSP.root"
##fname = "output_MyPhysicsList.root"
fname = "output_ThermalBeam_0_0_4.root"
fTrees = read_file(fname)
data_array = []
branchMap = {}
branches = {}
if(fTrees):
    keys = list(fTrees.keys())                                          # Get the list of Tree names in the file
    if(keys):                                                           # If no particles traverse then no tree is created. Hence checking if the trees exist    
        for key in keys:                                                # Read the number of particles transmitting to sensitive volume
            if(fTrees[key][0]):
                data_array = fTrees[key][3]
                branchMap = fTrees[key][2]
                branches = fTrees[key][1]

EDepAlpha = np.array([])
EDepTrit = np.array([])
EDepNet = np.array([])
particles = []
processes = []
evtAl = []
evtTr = []

if(data_array):
    fEvtCol = branchMap['fEvtNo']
    fCopyCol = branchMap['fCopyNo']
    fMatCol = branchMap['fMaterial']
    fPartCol = branchMap['fParticle']
    fParentCol = branchMap['fParentID']
    fProcCol = branchMap['fProdProcess']
    fEDepCol = branchMap['fEnergyDep']
    ongoingevtal = 0
    ongoingevtnet = 0
    ongoingevttr = 0
    edepevtal = 0.0
    edepevttr = 0.0
    edepevtnet = 0.0
    for i,data in enumerate(data_array):
        if(i%100000 == 0):
            print(f"Analysed Entry {i}")
            print(f"Number of Events Analysed: {data[fEvtCol]}")
            print(f"Num of Alpha Events: {len(EDepAlpha)}")
            print(f"Num of Triton Events: {len(EDepTrit)}")
        currevtal = data[fEvtCol]
        currevttr = data[fEvtCol]
        currevtnet = data[fEvtCol]
        if data[fPartCol] not in particles:
            particles.append(data[fPartCol])

        if(data[fPartCol] == 'alpha' and data[fProcCol] == 'neutronInelastic'):
            #print(data)
##            if currevtal not in evtAl:
##                evtAl.append(currevtal)
            if(currevtal == ongoingevtal):
                edepevtal += data[fEDepCol]
            else:
                EDepAlpha = np.append(EDepAlpha,edepevtal)
                edepevtal = data[fEDepCol]
                ongoingevtal = currevtal
        
        if(data[fPartCol] == 'triton' and data[fProcCol] == 'neutronInelastic'):
            ##print(data)
##            if currevttr not in evtTr:
##                evtTr.append(currevttr)
            if(currevttr == ongoingevttr):
                ##print(data)
                edepevttr += data[fEDepCol]
            else:
                EDepTrit = np.append(EDepTrit,edepevttr)
                edepevttr = data[fEDepCol]
                ongoingevttr = currevttr

        if(data[fPartCol] == 'alpha' or data[fPartCol] == 'triton' and data[fProcCol] == 'neutronInelastic'):
            #print(data)
##            if currevtal not in evtAl:
##                evtAl.append(currevtal)
            if(currevtnet == ongoingevtnet):
                edepevtnet += data[fEDepCol]
            else:
                EDepNet = np.append(EDepNet,edepevtnet)
                edepevtnet = data[fEDepCol]
                ongoingevtnet = currevtnet
##    
print("Triton",len(EDepTrit))
print("Alpha",len(EDepAlpha))
print("Alpha",len(EDepNet))

# Start an interactive interpreter session with access to local variables
code.interact(local=locals())
