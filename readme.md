Description of notebooks
The names of files are renamed to 1-XXX 2-XXX 3-XXX where 1, 2, 3 represnt the sequence in which the notebooks are to be run. Here is a little information about the notebooks
1-RawDownloader: This file downloads the files containing vulnerable functions. It downloads before-fix and after-fix versions of the file in files/before and files/after directories

step 1a: you need to run a joern command to export the information of all the methods in methods.json. It is from here know the functions linestart and line end number

2-DataAggregator: This file extracts vulnerable functions from the downloaded files. Generates patch information and stores them a nice dataframe and pickles it

3-AddDiffAdjacencyEdges: This file takes methods and adds edges to and from patch to the original file

4-InputEncoder: This just generates the embeddings for the input files. 