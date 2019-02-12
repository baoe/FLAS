#!/usr/bin/env python
#by bao: to let runFLAS.py use this script directly
# Finds all maximal cliques in a graph using the Bron-Kerbosch algorithm. The input graph here is 
# in the adjacency list format, a dict with vertexes as keys and lists of their neighbors as values.
# https://en.wikipedia.org/wiki/Bron-Kerbosch_algorithm

from collections import defaultdict
import string
from sys import argv
#######################################
'''
graphh = {
  1:[2,5],
  2:[1,3,5],
  3:[2,4],
  4:[3,5,6],
  5:[1,2,4],
  6:[4]
}
'''
#######################################
fin = open(argv[1],'r')
graph = defaultdict(list)
linenum = 0

#by bao: numline counts number of alignments
numline = 0
for line in fin:
  numline+=1
fin.seek(0)

for line in fin:
  if int(line.split("\t")[1]) not in graph[int(line.split("\t")[0])]:
    graph[int(line.split("\t")[0])].append(int(line.split("\t")[1]))
  if int(line.split("\t")[0]) not in graph[int(line.split("\t")[1])]:  
    graph[int(line.split("\t")[1])].append(int(line.split("\t")[0]))
  #if string.atoi(line.split("\t")[1]) not in graph[string.atoi(line.split("\t")[0])]:
 #   graph[string.atoi(line.split("\t")[0])].append(string.atoi(line.split("\t")[1]))
 # if string.atoi(line.split("\t")[0]) not in graph[string.atoi(line.split("\t")[1])]:  
  #  graph[string.atoi(line.split("\t")[1])].append(string.atoi(line.split("\t")[0]))
  linenum+=1

#by bao: linenum samples the alignments, and multiple samples forming all the alignments will be processed in parallel in updated versions
  if linenum > numline/int(argv[4])*5: break

  #if linenum%1000 == 0 :
   # print(linenum)


fin.close()


#print('graph\n')



########################################
cliques = []
countrecursion1=0
countrecursion2=0

####################################

def find_cliques(graph):
  p = set(graph.keys())
  r = set()
  x = set()
  global countrecursion1
  global countrecursion2
  ordering = []
  ordering = degeneracy_ordering(graph)
  for v in ordering:
    #print('%d \tin \t %d' %(v,len(graph)))
    countrecursion2=0
    countrecursion1+=1
   # if countrecursion1<len(ordering)*0.5:
    #  continue

    neighs = graph[v]
    find_cliques_pivot(graph, r.union([v]), p.intersection(neighs), x.intersection(neighs), cliques)
    p.remove(v)
    x.add(v)
  return sorted(cliques, key = lambda x: len(x), reverse = True)

def find_cliques_pivot(graph, r, p, x, cliques):
  global countrecursion1
  global countrecursion2
  #if countrecursion2 % 10000 == 0:
    #print('len(p) = %d \tlen(x) = %d \t countrecursion1 = %d \tin \t %d ,\t countrecursion2 = %d' %(len(p),len(x),countrecursion1,len(graph),countrecursion2))
  if countrecursion2 > 100:
    return
  countrecursion2+=1
  if len(p) == 0 and len(x) == 0:
    cliques.append(r)
  else:
    #u = iter(p.union(x)).next()
    u = iter(p.union(x)).__next__()
    for v in p.difference(graph[u]):
      neighs = graph[v]
      find_cliques_pivot(graph, r.union([v]), p.intersection(neighs), x.intersection(neighs), cliques)
      p.remove(v)
      x.add(v)

def degeneracy_ordering(graph):
  ordering = []
  ordering_set = set()
  degrees = defaultdict(lambda : 0)
  degen = defaultdict(list)
  max_deg = -1
  for v in graph:
    deg = len(graph[v])
    degen[deg].append(v)
    degrees[v] = deg
    if deg > max_deg:
      max_deg = deg

  while True:
    i = 0
    while i <= max_deg:
      if len(degen[i]) != 0:
        break
      i += 1
    else:
      break
    v = degen[i].pop()
    ordering.append(v)
    ordering_set.add(v)
    for w in graph[v]:
      if w not in ordering_set:
        deg = degrees[w]
        degen[deg].remove(w)
        if deg > 0:
          degrees[w] -= 1
          degen[deg - 1].append(w)
#  print('degeneracy_ordering(graph)\n')
  ordering.reverse()

  return ordering

find_cliques(graph)
#print('find_cliques(graph)\n')
fout1 = open(argv[2],'w')
fout2 = open(argv[3],'w')

cliquenum = 0
edgenum = 0
read0 = 0
read1 = 0
countreadname = 0 
for c in cliques:
  if len(c) > 10 :
    if len(c)<100:
      for j in c:
        fout1.writelines(str(j))
        fout1.writelines(" ")
      cliquenum += 1
      fout1.writelines("\n")
      if cliquenum > 1000 :
        break
  elif len(c) == 2:
    for j in c:
      fout2.writelines(str(j))
      fout2.writelines(" ")
    edgenum += 1
   # if edgenum % 1000 == 0:
     # print('edgenum = ', edgenum )
    fout2.writelines("\n")

  
fout1.close()

fout2.close()






