import networkx as nx

if __name__ == "__main__":
    G = nx.DiGraph() # create empty graph
    file = open("datad7.txt","r")
    sLetters = file.read()
    lNodes = []
    # create list of nodes
    for line in sLetters.split('\n'):
        for letter in line.split(' '):
            lNodes.append(letter)

    # create list of edges
    lEdges = [list(x) for x in sLetters.split('\n')]
    for x in lEdges:
        x.remove(' ') # remove whitespaces from lists

    # add to graph
    G.add_nodes_from(lNodes)
    G.add_edges_from(lEdges)

    print(G.adj)
    path = nx.topological_sort(G)
    for x in path:
        print(x)