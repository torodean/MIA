/**
 * @file LOLANetUse.hpp
 * @author Antonius Torode
 * @date 09/28/2016 (originally for LOLA), updated 06/12/2017 (for MIA)
 * Description: Header file for the LOLANetUse.cpp implementation.
 */
#pragma once

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
