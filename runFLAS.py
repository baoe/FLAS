#!/usr/bin/python

import os
import argparse
import shutil
import datetime

parser = argparse.ArgumentParser(description='runHALC.py')
parser.add_argument('long_read_path', metavar='long_read.fa', help="The path to long_read.fa")
parser.add_argument('-c', help="Coverage of the long reads.(100)", default=100,type=int)
parser.add_argument('-a1',help="Fraction value to distinguish between the cases (i)/(ii) and (iii).(0.5)", default=0.5,type=float)
parser.add_argument('-a2',help="Fraction value to distinguish between the cases (i) and (ii).(0.5)", default=0.5,type=float)
parser.add_argument('-b1', help="Difference value of alignment identities to find the correct aligned path.(0.05)", default=0.05,type=float)
parser.add_argument('-b2', help="Difference value of expected amounts of aligned long reads to find the correct aligned path.(0.2)", default=0.2,type=float)
args = parser.parse_args()

# Default Parameters#####################################
temp_dir = './temp'
output_dir = './output'
prefix = 'FLAS'
repeat_free_mode = False

if os.path.exists(temp_dir + '/step1'):
	if os.path.exists(temp_dir + '/step2'):
		print ('WARNING: ' + temp_dir + '/step2' + ' was found. Automatically started from step2')
		shutil.rmtree(temp_dir + '/step2')
		start_from_step = 2
	else:
		print ('WARNING: ' + temp_dir + '/step1' + ' was found. Automatically started from step1')
		shutil.rmtree(temp_dir + '/step1')
		start_from_step = 1
else:
	start_from_step = 1
# Parameters Analyzing###################################
long_read_path = args.long_read_path

if args.a1 > 1 or args.a1 < 0:
	print ('ERROR: Argument -a1  INVALID INPUT PARAMETER (should be between 0 and 1)!')
	exit(-1)
if args.a2 > 1 or args.a2 < 0:
	print ('ERROR: Argument -a2  INVALID INPUT PARAMETER (should be between 0 and 1)!')
	exit(-1)
if args.b1 > 1 or args.b1 < 0:
	print ('ERROR: Argument -b1  INVALID INPUT PARAMETER (should be between 0 and 1)!')
	exit(-1)
if args.b2 > 1 or args.b2 < 0:
	print ('ERROR: Argument -b2  INVALID INPUT PARAMETER (should be between 0 and 1)!')
	exit(-1)

start_time = datetime.datetime.now()
print (start_time)

# Step 1 MECAT1#########################
if start_from_step <= 1:
	print('''
FLAS: Fast and High Throughput Algorithm for PacBio Long Read Self-Correction.

/////STEP 1 STARTED//////////////////////////////////////////////////////////////////////////////////////////////////''')

	if not os.path.exists(output_dir):
		os.makedirs(output_dir)

	if not os.path.exists(temp_dir):
		os.makedirs(temp_dir)

	if os.path.exists(temp_dir + '/step1'):
		print ('ERROR: ' + temp_dir + '/step1' + ' already exists, please delete it before running step 1')
		exit(-1)
	else:
		os.mkdir(temp_dir + '/step1')

	preread_command = 'preread ' + long_read_path + ' ' + temp_dir + '/step1/' + 'allreads.fasta ' 

	print ('Running command: ' + preread_command)
	err = os.system(preread_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run preread:' + os.strerror(err))
		exit(-1)

#################################
	mecat2pw1_command = 'mecat2pw -j 1 -t 16 -g 1 -x 0 -d ' + temp_dir + '/step1/' + 'allreads.fasta ' + ' -o ' + temp_dir + '/step1/' + 'allreads.fasta.m4 ' + ' -w ' + temp_dir + '/step1/' + 'tmpfold > ' + temp_dir + '/step1/mecat_log.txt 2> ' + temp_dir + '/step1/mecat_log.txt'

	print ('Running command: ' + mecat2pw1_command)

	err = os.system(mecat2pw1_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2pw1:' + os.strerror(err))
		exit(-1)

################################
	rm1_command = 'rm -rf ' + temp_dir + '/step1/' + 'tmpfold '

	print ('Running command: ' + rm1_command)

	err = os.system(rm1_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run rm1:' + os.strerror(err))
		exit(-1)

#####################################
	bronkerbosch1_command = 'bronkerboschadj ' + temp_dir + '/step1/' + 'allreads.fasta.m4 ' + temp_dir + '/step1/' + 'clique1.txt ' + temp_dir + '/step1/' + 'edge1.txt 3 '

	print ('Running command: ' + bronkerbosch1_command)
	err = os.system(bronkerbosch1_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run bronkerbosch:' + os.strerror(err))
		exit(-1)

#####################################
	FLAS1_command = 'FLAS 1 -c ' + temp_dir + '/step1/' + 'clique1.txt' + ' -e ' + temp_dir + '/step1/' + 'edge1.txt' + ' -r ' + temp_dir + '/step1/' + 'allreads.fasta' + ' -m ' + temp_dir + '/step1/' +'allreads.fasta.m4' + ' -a ' + temp_dir + '/step1/' +'realireads1.fasta' + ' -de1 ' + temp_dir + '/step1/' + 'deletepairs1.txt'
	print ('Running command: ' + FLAS1_command)
	err = os.system(FLAS1_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run FLAS1:' + os.strerror(err))
		exit(-1)
####################################
	
	mecat2pw2_command = ' mecat2pw -j 1 -t 16 -g 1 -x 0 -d ' + temp_dir + '/step1/' + 'realireads1.fasta ' + ' -o ' + temp_dir + '/step1/' + 'realireads1.fasta.m4 ' + ' -w ' + temp_dir + '/step1/' + 'tmpfold -k 2 -n 50 -a 1000 >> ' + temp_dir + '/step1/mecat_log.txt 2>> ' + temp_dir + '/step1/mecat_log.txt'


	print ('Running command: ' + mecat2pw2_command)
	err = os.system(mecat2pw2_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2pw2:' + os.strerror(err))
		exit(-1)

####################################
	rm2_command = 'rm -rf ' + temp_dir + '/step1/' + 'tmpfold '

	print ('Running command: ' + rm2_command)
	err = os.system(rm2_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run rm2:' + os.strerror(err))
		exit(-1)

#################################
	
	addm42MECATm41_command = 'addm42MECATm4 ' + temp_dir + '/step1/' + 'allreads.fasta.m4 ' + temp_dir + '/step1/' + 'realireads1.fasta.m4 ' + temp_dir + '/step1/' + 'realireads1.fasta ' +  temp_dir + '/step1/' + 'realireads1_.fasta.m4 '
	print ('Running command: ' + addm42MECATm41_command)
	err = os.system(addm42MECATm41_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run addm42MECATm41:' + os.strerror(err))
		exit(-1)

###############################

	mecat2cns1_command = 'mecat2cns -x 0 -i 1 -t 16 -l 10 -c 3 ' + temp_dir + '/step1/' + 'realireads1_.fasta.m4 ' + temp_dir + '/step1/' + 'allreads.fasta ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta >> ' + temp_dir + '/step1/mecat_log.txt 2>> ' + temp_dir + '/step1/mecat_log.txt'
	print ('Running command: ' + mecat2cns1_command)
	err = os.system(mecat2cns1_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2cns1:' + os.strerror(err))
		exit(-1)

#########################################
	print ('''
/////STEP 1 DONE/////////////////////////////////////////////////////////////////////////////////////////////////////''')
# Step 2 Run Blasr###############################
if start_from_step <= 2:
	print ('''
/////STEP 2 STARTED//////////////////////////////////////////////////////////////////////////////////////////////////''')
	if not os.path.exists(output_dir):
		os.makedirs(output_dir)

	if not os.path.exists(temp_dir):
		os.makedirs(temp_dir)

	if os.path.exists(temp_dir + '/step2'):
		print ('ERROR: ' + temp_dir + '/step2' + ' already exists, please delete it before running step 2')
		exit(-1)
	else:
		os.mkdir(temp_dir + '/step2')
###########################

	separatereads_command = 'separatereads ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta ' + temp_dir + '/step1/' + 'allreads.fasta ' + temp_dir + '/step2/' + 'incorrectedreads.fasta '
	print ('Running command: ' + separatereads_command)
	err = os.system(separatereads_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run separatereads:' + os.strerror(err))
		exit(-1)
#################################

	mecat2pw3_command = ' mecat2pw -j 1 -t 16 -g 1 -x 0 -d ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta ' + ' -o ' + temp_dir + '/step2/' + 'allreads0_corrected_filted1.fasta.m4 '+ ' -w ' + temp_dir + '/step2/' + 'tmpfold > ' + temp_dir + '/step2/mecat_log.txt 2> ' + temp_dir + '/step2/mecat_log.txt'
	print ('Running command: ' + mecat2pw3_command)
	err = os.system(mecat2pw3_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2pw3:' + os.strerror(err))
		exit(-1)
###########################################

	rm3_command = 'rm -rf ' + temp_dir + '/step2/' + 'tmpfold '
	print ('Running command: ' + rm3_command)
	err = os.system(rm3_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run rm3:' + os.strerror(err))
		exit(-1)

###########################################

	cat1_command = 'cat ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta '+ temp_dir + '/step2/' + 'incorrectedreads.fasta ' + '>' + temp_dir + '/step2/' + 'cor_incor.fasta '
	print ('Running command: ' + cat1_command)
	err = os.system(cat1_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run cat1:' + os.strerror(err))
		exit(-1)
#############################################

	mecat2pw4_command = ' mecat2pw -j 1 -t 16 -g 1 -x 0 -d ' + temp_dir + '/step2/' + 'cor_incor.fasta ' + ' -o ' + temp_dir + '/step2/' + 'cor_incor.fasta.m4 '+ ' -w ' + temp_dir + '/step2/' + 'tmpfold >> ' + temp_dir + '/step2/mecat_log.txt 2>> ' + temp_dir + '/step2/mecat_log.txt'
	print ('Running command: ' + mecat2pw4_command)
	err = os.system(mecat2pw4_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2pw4:' + os.strerror(err))
		exit(-1)

	rm4_command = 'rm -rf ' + temp_dir + '/step2/' + 'tmpfold '
	print ('Running command: ' + rm4_command)
	err = os.system(rm4_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run rm4:' + os.strerror(err))
		exit(-1)

########################################
	bronkerbosch2_command = 'bronkerboschadj ' + temp_dir + '/step2/' + 'allreads0_corrected_filted1.fasta.m4 ' + temp_dir + '/step2/' + 'clique2.txt ' +  temp_dir + '/step2/' + 'edge2.txt 3'
	print ('Running command: ' + bronkerbosch2_command)
	err = os.system(bronkerbosch2_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run bronkerbosch2:' + os.strerror(err))
		exit(-1)
############################################
	
	

	FN1_command = temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta'
	print ('Running command: ' + FN1_command)
	contents=open(FN1_command,"r").read()
	FN1= contents.count("\n")
	 
	
	

##########################################

	FLAS2_command = 'FLAS 2 '+ str(FN1) +  ' -c ' + temp_dir + '/step2/' + 'clique2.txt ' + ' -e ' + temp_dir + '/step2/' + 'edge2.txt ' + ' -r ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta ' + ' -m ' + temp_dir + '/step2/' +'allreads0_corrected_filted1.fasta.m4 ' + ' -de2 ' + temp_dir + '/step2/' +'deletepairs2.txt ' + ' -de3 ' + temp_dir + '/step2/' + 'deletepairs3.txt '+ ' -w ' + temp_dir + '/step2/' + 'cor_incor.fasta ' + ' -g ' + temp_dir + '/step2/' + 'cor_incor.fasta.m4 ' +'-C ' + str(args.c)  +'-a1 '+str(args.a1)+' -a2 '+str(args.a2)+' -b1 '+str(args.b1)+' -b2 '+str(args.b2)
	print ('Running command: ' + FLAS2_command)
	err = os.system(FLAS2_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run FLAS2:' + os.strerror(err))
		#exit(-1)
#########################################
	modifym4file2_command = 'modifym4file2 ' + temp_dir + '/step2/' + 'cor_incor.fasta.m4 '+ temp_dir + '/step2/' + 'deletepairs3.txt ' + temp_dir + '/step2/' + 'cor_incor.fasta '+ temp_dir + '/step2/' +'corrected2.m4 '
	print ('Running command: ' + modifym4file2_command)
	err = os.system(modifym4file2_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run modifym4file2:' + os.strerror(err))
		exit(-1)
######################################
	mecat2cns2_command = 'mecat2cns -x 0 -i 1 -t 16 -l 10 -c 1 ' + temp_dir + '/step2/' + 'corrected2.m4 ' + temp_dir + '/step2/' + 'cor_incor.fasta ' + temp_dir + '/step2/' + 'allreads0_corrected_filted2.fasta >> ' + temp_dir + '/step2/mecat_log.txt 2>> ' + temp_dir + '/step2/mecat_log.txt'
	print ('Running command: ' + mecat2cns2_command)
	err = os.system(mecat2cns2_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2cns2:' + os.strerror(err))
		exit(-1)

##########################################

	filter_command = 'filter ' + temp_dir + '/step2/' + 'allreads0_corrected_filted2.fasta ' + str(FN1) + ' ' + temp_dir + '/step2/' + 'allreads0_corrected_filted3.fasta '
	print ('Running command: ' + filter_command)
	err = os.system(filter_command)

	if err != 0:
		print ('ERROR: ' + 'Failed to run mecat2cns2:' + os.strerror(err))
		exit(-1)
##############################################

	cat3_command = 'cat ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta '+ temp_dir + '/step2/' + 'allreads0_corrected_filted3.fasta ' + '>' + output_dir + '/split_reads.fasta '
	print ('Running command: ' + cat3_command)
	err = os.system(cat3_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run cat3:' + os.strerror(err))
		exit(-1)



#############################################
#	full_trim_command = 'full_trim ' + output_dir + '/split_reads.fasta '+ temp_dir + '/step1/' + 'allreads.fasta '+ output_dir +'/full_reads.fasta '  + output_dir +'/trimmed_reads.fasta
#by bao: split_reads.fasta does not directly correspond to allreads.fasta
	full_trim_command = 'full_trim ' + temp_dir + '/step1/' + 'allreads0_corrected_filted1.fasta ' + temp_dir + '/step1/' + 'allreads.fasta ' + temp_dir + '/step2/' + 'full_reads1.fasta ' + temp_dir + '/step2/' + 'trimmed_reads1.fasta '
	print ('Running command: ' + full_trim_command)
	err = os.system(full_trim_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run full_trim:' + os.strerror(err))
		exit(-1)
	full_trim_command = 'full_trim ' + temp_dir + '/step2/' + 'allreads0_corrected_filted3.fasta ' + temp_dir + '/step2/' + 'cor_incor.fasta ' + temp_dir + '/step2/' + 'full_reads2.fasta ' + temp_dir + '/step2/' + 'trimmed_reads2.fasta '
	print ('Running command: ' + full_trim_command)
	err = os.system(full_trim_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run full_trim:' + os.strerror(err))
		exit(-1)
	cat4_command = 'cat ' + temp_dir + '/step2/' + 'trimmed_reads1.fasta '+ temp_dir + '/step2/' + 'trimmed_reads2.fasta ' + '>' + output_dir + '/trimmed_reads.fasta '
	print ('Running command: ' + cat4_command)
	err = os.system(cat4_command)
	if err != 0:
		print ('ERROR: ' + 'Failed to run cat4_command:' + os.strerror(err))
		exit(-1)

print ('''


/////Finished!!! Results are stored in output folder/////////////////////////////////////////////////////////////////''')

end_time = datetime.datetime.now()
print (end_time)
time_cost = end_time - start_time
print ('Running time: ' + str(time_cost))

