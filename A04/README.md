For read_ppm, it appears verbose because I performed a lot of error checking
for formatting errors in the ppm files, but maybe I should have just
said there would be undefined behavior if the ppm file was not correct? 
Earlier commits showed I did not check for those errors, but the checks do not
hurt I think. The program would be less verbose if I did not have to check
for the errors. Maybe for future assignments, I should just optimize the read,
so that it assumes the user inputs a correct file...
