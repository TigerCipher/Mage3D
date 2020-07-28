# // Mage3D Build Script
# // Copyright (C) 2020 Blue Moon Development
# // This program is free software: you can redistribute it and/or modify
# // it under the terms of the GNU General Public License as published by
# // the Free Software Foundation, either version 3 of the License, or
# // (at your option) any later version.
# // This program is distributed in the hope that it will be useful,
# // but WITHOUT ANY WARRANTY; without even the implied warranty of
# // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# // GNU General Public License for more details.
# // You should have received a copy of the GNU General Public License
# // along with this program. If not, see <http://www.gnu.org/licenses/>.
# // 
# // Contact: team@bluemoondev.org
# // 
# // File Name: builder.py
# // Author: Matt / TigerCipher


import os
import sys
import platform
import subprocess


RESET = '\033[m'
BLACK = '\033[30m'
RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
BLUE = '\033[34m'
MAGENTA = '\033[35m'
CYAN = '\033[36m'
WHITE = '\033[37m'

BOLD_RED = '\033[1;31m'
ERR_COLOR = BOLD_RED



class GeneratorFinder:
	def __init__(self):
		helpList = []
		res = subprocess.run(['cmake', '--help'], stdout=subprocess.PIPE)
		retList = []
		retPrintList = []
		isGen = False
		index = 0
		for item in res.stdout.decode('utf-8').split('\n'):
			if 'Generators' in item:
				isGen = True
			if isGen and item != 'Generators' and item != '':
				if 'default' in item:
					retPrintList.append(item)
				else:
					if ('Visual' in item or 'Borland' in item or 'NMake Makefiles' in item or 'MSYS' in item or 
						'MinGW Makefiles' in item or 'Green' in item or 'Ninja' in item or 
						'Unix Makefiles' in item or 'Watcom' in item):
						helpList.append(item)
						asString = '[' + str(index) + ']: ' + item
						retPrintList.append(asString)
						index += 1
					else:
						retPrintList.append('\t' + item)
		self.maxIndex = index
		defIndex = 0
		for v in helpList:
			st = -1
			ind = v.find('=')
			if '[' in v:
				ind = v.find('[')
			if '*' in v:
				st = v.find('*')
				self.defaultIndex = defIndex
			if ind == -1:
				stripped = v.strip()
			else:
				stripped = v[st + 1:ind].strip()
			defIndex += 1
			retList.append(stripped)
		self.generators = retList
		self.printables = retPrintList


def main():
	# Defaults

	# Still way too early in development to even consider giving option to make a Release build
	# buildType = 'Debug'
	osName = platform.system()

	print('Detected Operating System ' + YELLOW + osName + RESET + ', preparing build...\n')
	print('Enter a name for the build. eg: VStudio is the name I like when working with Visual Studio. The name will be prefixed by \"Build\"')
	var = input(CYAN + 'Build...' + RESET)
	name = "Build" + var

	propsFile = './mage.properties'

	useOldSettings = False
	if os.path.exists(propsFile):
		print(YELLOW + 'Would you like to build with these settings again?' + RESET)
		seperator = '='
		keys = {}

		with open(propsFile) as f1:
			for line in f1:
				print(CYAN + line + RESET)
				if seperator in line:
					key, value = line.split(seperator, 1)
					keys[key.strip()] = value.strip()
		oldSettings = input("Keep these settings? [y,n]: ") or 'y'
		if oldSettings.lower() == 'y' or oldSettings.lower() == 'yes':
			generator = keys['CMAKE_GENERATOR']
			toolchain = keys['CMAKE_TOOLCHAIN_FILE']
			useOldSettings = True

	if not useOldSettings:
		finder = GeneratorFinder()
		genList = finder.generators

		print('Select the generator you wish to create project files and build Mage3D with, or leave blank to use your cmake default\n')
		print('Would you like to see a list of cmake generators?')
		print('YES = y or yes... NO = n or no')
		viewList = input('[y,n]: ') or 'y'

		if viewList.lower() == 'y' or viewList.lower() == 'yes':
			print('\n=========================== Generators Available ======================================\n')

			for var in finder.printables:
				print(var)

			print('\n======================================================================================\n')

			print('Again, leave the generator option blank to use the default')
			print('Use the index number')
			gen = int(input('Which generator? ') or '0')
			if gen >= finder.maxIndex:
				maxAllowed = finder.maxIndex - 1
				print(ERR_COLOR + 'Index specified must be no greater than ' + str(maxAllowed) + RESET)
				sys.exit(2)
			elif gen < 0:
				print(ERR_COLOR + 'Index specified must be no less than 0' + RESET)
				sys.exit(2)
		else:
			print('Again, leave the generator option blank to use the default')
			print('Otherwise, include the full name as accepted by cmake')
			genStr = input('Which generator? ')
			genFound = False
			i = 0
			if genStr == '':
				gen = finder.defaultIndex
				genFound = True
			else:
				for var in finder.generators:
					if genStr == var:
						genFound = True
						gen = i
						break
					i += 1

			if not genFound:
				print(ERR_COLOR + 'The specified generator', genStr, 'does not match those supported by cmake' + RESET)
				sys.exit(2)
		if osName == 'Linux':
			vcpkgHome = '~/'
		elif osName == 'Darwin':
			vcpkgHome = '/Library/'
		elif osName == 'Windows':
			vcpkgHome = 'C://dev/'
		
		print('Default VCPKG installation directory = ' + CYAN + vcpkgHome + RESET + '\n')
		print('If this is incorrect, please specify the correct location. The /vcpkg/ will get automatically added\n')
		print('In other words, the toolchain file cmake will use will be ' + CYAN + '(your path)/vcpkg/scripts/buildsystems/vcpkg.cmake\n' + RESET)
		newLoc = input('Enter the correct path, or leave blank if its already correct: ')
		if newLoc != '':
			vcpkgHome = newLoc

		print('Setting cmake toolchain file to ' + CYAN + vcpkgHome + '/vcpkg/scripts/buildsystems/vcpkg.cmake\n' + RESET)
		toolchain = vcpkgHome + '/vcpkg/scripts/buildsystems/vcpkg.cmake'

		generator = finder.generators[gen]


	print('Setting build properties as follows: ')
	print('CMAKE_BINARY_DIR (build folder name) = ' + MAGENTA + name + RESET)
	print('CMAKE_GENERATOR = ' + MAGENTA + generator + RESET)
	print('CMAKE_TOOLCHAIN_FILE = ' + MAGENTA + toolchain + RESET)

	confirm = input(YELLOW + 'Is this all correct? [y,n]: ' + RESET) or 'y'
	if confirm.lower() != 'y' and confirm.lower() != 'yes':
		print(RED + "Script will now exit, re-run to correct your settings" + RESET)
		sys.exit()

	if not useOldSettings:
		f = open(propsFile, 'w')
		f.write('CMAKE_GENERATOR=' + generator + '\n')
		f.write('CMAKE_TOOLCHAIN_FILE=' + toolchain)
		f.close()

	print(GREEN + 'Now building the project...')
	cmd = 'cmake -S . -B ' + name + ' -G \"' + generator + '\" -DCMAKE_TOOLCHAIN_FILE=' + toolchain
	print('Running command: ' + cmd + RESET)
	os.system(cmd)
 


if __name__ == "__main__":
	main()