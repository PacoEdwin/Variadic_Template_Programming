Problem:
You have various genes that implement similiar interfaces and you want enable user or developer to define list of genes that program will use. 

tt_Gene.* allows to set class like Gene<Gene1, Gene5, Gene3> and then your future organism will have your upper class defining and using all of the data from its subclasses(Gene1, Gene5, Gene3). While genes in your disposal are Gene1,Gene2,...,Gene10...

C++11 used