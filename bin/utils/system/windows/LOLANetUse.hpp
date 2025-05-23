//============================================================================
// Name        : LOLANetUse.h
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : September 28, 2016 (LOLA) -- June 12, 2017 (MIA)
// Description : The header file for the LOLANetUse.cpp file.
//============================================================================

class LOLANetUse
{
	public:
		LOLANetUse();
		~LOLANetUse();
		
		//Functions adapted to work from the old LOLA Program.
		void NetSessionEnumRunner(int argc, wchar_t *argv[]); 
		void NetServerEnumRunner_WKST(int argc, wchar_t *argv[]); 
		void NetServerEnumRunner_SERV(int argc, wchar_t *argv[]); 
		void NetWkstaGetInfoRunner(int argc, wchar_t *argv[]); 
		void NetRemoteComputerSupportsRunner(int argc, wchar_t *argv[]); 
		int NetUserEnumRunner(int argc, wchar_t *argv[]); 
		int NetUserGetInfoRunner(int argc, wchar_t *argv[]); 
		int NetUserModalsGetRunner(int argc, wchar_t *argv[]); 
		int NetQueryDisplayInformationRunner(wchar_t *argv[], int level); 
};
