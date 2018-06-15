#LIBRARY PROJECTS
#----------------------------------
PROJECT(Libraries)

ADD_SUBDIRECTORY(others)
message("ADD_SUBDIRECTORY(others) - done!")

#Librerie shared
		
ADD_SUBDIRECTORY(egeshare)	
message("ADD_SUBDIRECTORY(egeshare) - done!")

ADD_SUBDIRECTORY(ege2d)
message("ADD_SUBDIRECTORY(ege2d) - done!")


