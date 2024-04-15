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
        for i, branch in enumerate(branches):
            branch_name = branch.GetName()
            branchIndex_map[i] = branch_name

        # Create an empty NumPy array to store the data
        data_array = []

        # Loop through each event in the tree
        for i, event in enumerate(tree):
            data_array.append([])
            # Loop through each branch in the tree
            for j in range(numBranches):
                # Get the value of the branch for the current event and store it in the data array
                data_array[i].append(getattr(event, f"{branchIndex_map[j]}"))
        
        return branchIndex_map, data_array, numEntries
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
            fTree_name = key.GetName()                                      # Get the TTree name
            branchIndex_map, data_array, numEntries = read_tree(obj)        # Read the TTree
            fTrees[fTree_name] = (numEntries, branchIndex_map, data_array)  # Store the output in a dictionary

    return fTrees

##################################################################################################################
##                                             MAIN PROGRAM                                                     ##
##################################################################################################################

# Variable containing the required data
numParticleInSensVol = []

##currEnergy = minEnergy
##while(currEnergy <= maxEnergy):                                             # Loop over the energies
fname = "output0.root"
fTrees = read_file(fname)
data_array = {}
if(fTrees):
    keys = list(fTrees.keys())                                          # Get the list of Tree names in the file
    if(keys):                                                           # If no particles traverse then no tree is created. Hence checking if the trees exist    
        for key in keys:                                                # Read the number of particles transmitting to sensitive volume
            if(fTrees[key][0]):
                data_array[key] = fTrees[key][2]

EDepAlpha = []
EDepTrit = []
particles = []
processes = []
if(data_array):
    keys = list(data_array.keys())
    key = 'LiF'
    ongoingevtal = 0
    ongoingevttr = 0
    edepevtal = 0.0
    edepevttr = 0.0
    for i,data in enumerate(data_array[key]):
        currevt = data[0]
        if data[2] not in particles:
            particles.append(data[2])
        if data[2] == 'triton' and data[4] not in processes:
            processes.append(data[4])
        if(data[2] == 'alpha' and data[3] == 1 and data[1] == 5 and data[4] == 'neutronInelastic'):
            #print(data)
            if(currevt == ongoingevtal):
                edepevtal += data[5]
            else:
                EDepAlpha.append(edepevtal)
                edepevtal = data[5]
                ongoingevtal = currevt
        
        if(data[2] == 'triton' and data[3] == 1 and data[1] == 5 and data[4] == 'neutronInelastic'):
            ##print(data)
            if(currevt == ongoingevttr):
                ##print(data)
                edepevttr += data[5]
            else:
                EDepTrit.append(edepevttr)
                edepevttr = data[5]
                ongoingevttr = currevt
    
print("Triton",len(EDepTrit))
print("Alpha",len(EDepAlpha))

# Start an interactive interpreter session with access to local variables
code.interact(local=locals())
