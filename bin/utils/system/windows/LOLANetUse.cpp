/**
 * @file LOLANetUse.cpp
 * @author Antonius Torode
 * @date 09/28/2016 (original LOLA), updated 06/12/2017 (adapted for MIA)
 * Description: The LOLANetUse functions from the LOLA program adapted for use with MIA.
 */

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <cstdio>
#include <cassert>
#include <lm.h>
#include <stdexcept>
#include "Sddl.h"
#include "LOLANetUse.hpp"

//Main constructor for the LOLANetUse class.
LOLANetUse::LOLANetUse()
{
	//Chuck Norris can divide by 0.
}

//Main deconstructor for the LOLANetUse class.
LOLANetUse::~LOLANetUse()
{
	//There is no Esc key on Chuck Norris’ keyboard, because no one escapes Chuck Norris.
}

//The NetSessionEnum function Provides information about sessions established on a server.
//Taken from https://msdn.microsoft.com/en-us/library/windows/desktop/bb525382(v=vs.85).aspx
void LOLANetUse::NetSessionEnumRunner(int argc, wchar_t *argv[])
{
	LPSESSION_INFO_10 pBuf = nullptr, pTmpBuf;
	DWORD dwLevel = 10, dwPrefMaxLen = MAX_PREFERRED_LENGTH, dwEntriesRead = 0, dwTotalEntries = 0, dwResumeHandle = 0, i, dwTotalCount = 0;
	LPTSTR pszServerName = nullptr, pszClientName = nullptr, pszUserName = nullptr;
	NET_API_STATUS nStatus;

	// Check command line arguments.
	if (argc > 4){
		wprintf(L"Usage: %s [\\\\ServerName] [\\\\ClientName] [UserName]\n", argv[0]);
		exit(1);
	}
	if (argc >= 2){
		pszServerName = argv[1];
	}
	if (argc >= 3){
		pszClientName = argv[2];
	}
	if (argc == 4){
		pszUserName = argv[3];
	}
	
	// Call the NetSessionEnum function, specifying level 10.
	do { // begin do
		nStatus = NetSessionEnum(pszServerName, pszClientName, pszUserName, dwLevel, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);

		// If the call succeeds,
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
			if ((pTmpBuf = pBuf) != nullptr){ 
				// Loop through the entries.
				for (i = 0; i < dwEntriesRead; i++){
					assert(pTmpBuf != nullptr);

					if (pTmpBuf == nullptr){
						fprintf(stderr, "...An access violation has occurred\n");
						break;
					}

					// Print the retrieved data. 
					wprintf(L"\n\tClient: %ls\n", pTmpBuf->sesi10_cname);
					wprintf(L"\tUser:   %ls\n", pTmpBuf->sesi10_username);
					printf("\tActive: %d\n", pTmpBuf->sesi10_time);
					printf("\tIdle:   %d\n", pTmpBuf->sesi10_idle_time);
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		} else { // Otherwise, indicate a system error.
			fprintf(stderr, "...A system error has occurred: %d\n", nStatus);
		}
		
		// Free the allocated memory.
		if (pBuf != nullptr){
			NetApiBufferFree(pBuf);
			pBuf = nullptr;
		}
	}

	// Continue to call NetSessionEnum while there are more entries. 
	while (nStatus == ERROR_MORE_DATA); // end do

	// Check again for an allocated buffer.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
	
	// Print the final count of sessions enumerated.
	fprintf(stderr, "\n...Total of %d entries enumerated\n", dwTotalCount);
}

//The NetServerEnum function lists all workstations that are visible in a domain. 
void LOLANetUse::NetServerEnumRunner_WKST(int argc, wchar_t *argv[])
{
	LPSERVER_INFO_101 pBuf = nullptr;
	LPSERVER_INFO_101 pTmpBuf;
	DWORD dwLevel = 101;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwTotalCount = 0;
	DWORD dwServerType = SV_TYPE_WORKSTATION;        // all workstations
	DWORD dwResumeHandle = 0;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = nullptr;
	LPWSTR pszDomainName = nullptr;
	DWORD i;

	if (argc > 2){
		fwprintf(stderr, L"...Usage: %s [DomainName]\n", argv[0]);
		exit(1);
	}
	// The request is not for the primary domain.
	if (argc == 2){
		pszDomainName = argv[1];
	}
	
	// Call the NetServerEnum function to retrieve information
	//  for all servers, specifying information level 101.
	nStatus = NetServerEnum(pszServerName, dwLevel, (LPBYTE *)& pBuf, dwPrefMaxLen,
		&dwEntriesRead, &dwTotalEntries, dwServerType, pszDomainName, &dwResumeHandle);

	// If the call succeeds,
	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
		if ((pTmpBuf = pBuf) != nullptr) {
			// Loop through the entries and print the data for all server types.
			for (i = 0; i < dwEntriesRead; i++) {
				assert(pTmpBuf != nullptr);

				if (pTmpBuf == nullptr) {
					fprintf(stderr, "...An access violation has occurred\n");
					break;
				}

				printf("\tPlatform: %d\n", pTmpBuf->sv101_platform_id);
				wprintf(L"\tName:     %ls\n", pTmpBuf->sv101_name);
				printf("\tVersion:  %d.%d\n", pTmpBuf->sv101_version_major, pTmpBuf->sv101_version_minor);
				printf("\tType:     %d", pTmpBuf->sv101_type);

				// Check to see if the server is a domain controller;
				//  if so, identify it as a PDC or a BDC.
				if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_CTRL){
					wprintf(L" (PDC)");
				} else if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_BAKCTRL) {
					wprintf(L" (BDC)");
				}
				printf("\n");

				// Also print the comment associated with the server.
				wprintf(L"\t...Comment:  %ls\n\n", pTmpBuf->sv101_comment);

				pTmpBuf++;
				dwTotalCount++;
			}
			// Display a warning if all available entries were
			//  not enumerated, print the number actually 
			//  enumerated, and the total number available.

			if (nStatus == ERROR_MORE_DATA) {
				fprintf(stderr, "\n...More entries available!!!\n");
				fprintf(stderr, "...Total entries: %d", dwTotalEntries);
			}

			printf("\nEntries enumerated: %d\n", dwTotalCount);

		} else {
			printf("...No servers were found\n");
			printf("...The buffer (bufptr) returned was nullptr\n");
			printf("...entriesread: %d\n", dwEntriesRead);
			printf("...totalentries: %d\n", dwEntriesRead);
		}

	} else {
		fprintf(stderr, "...NetServerEnum failed with error: %d\n", nStatus);
	}
	// Free the allocated buffer.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
}

//The NetServerEnum function lists all servers that are visible in a domain. 
void LOLANetUse::NetServerEnumRunner_SERV(int argc, wchar_t *argv[])
{
	LPSERVER_INFO_101 pBuf = nullptr;
	LPSERVER_INFO_101 pTmpBuf;
	DWORD dwLevel = 101;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwTotalCount = 0;
	DWORD dwServerType = SV_TYPE_SERVER; // all servers
	DWORD dwResumeHandle = 0;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = nullptr;
	LPWSTR pszDomainName = nullptr;
	DWORD i;

	if (argc > 2){
		fwprintf(stderr, L"...Usage: %s [DomainName]\n", argv[0]);
		exit(1);
	}
	// The request is not for the primary domain.
	if (argc == 2){
		pszDomainName = argv[1];
	}
	// Call the NetServerEnum function to retrieve information
	//  for all servers, specifying information level 101.
	nStatus = NetServerEnum(pszServerName, dwLevel, (LPBYTE *)& pBuf, dwPrefMaxLen,
		&dwEntriesRead, &dwTotalEntries, dwServerType, pszDomainName, &dwResumeHandle);

	// If the call succeeds,
	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
		if ((pTmpBuf = pBuf) != nullptr) {
			// Loop through the entries and print the data for all server types.
			for (i = 0; i < dwEntriesRead; i++){
				assert(pTmpBuf != nullptr);

				if (pTmpBuf == nullptr){
					fprintf(stderr, "...An access violation has occurred\n");
					break;
				}

				printf("\tPlatform: %d\n", pTmpBuf->sv101_platform_id);
				wprintf(L"\tName:     %ls\n", pTmpBuf->sv101_name);
				printf("\tVersion:  %d.%d\n", pTmpBuf->sv101_version_major, pTmpBuf->sv101_version_minor);
				printf("\tType:     %d", pTmpBuf->sv101_type);

				// Check to see if the server is a domain controller;
				//  if so, identify it as a PDC or a BDC.
				if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_CTRL){
					wprintf(L" (PDC)");
				} else if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_BAKCTRL){
					wprintf(L" (BDC)");
				}

				printf("\n");

				// Also print the comment associated with the server.
				wprintf(L"\tComment:  %ls\n\n", pTmpBuf->sv101_comment);

				pTmpBuf++;
				dwTotalCount++;
			}
			// Display a warning if all available entries were
			//  not enumerated, print the number actually 
			//  enumerated, and the total number available.

			if (nStatus == ERROR_MORE_DATA) {
				fprintf(stderr, "\n...More entries available!!!\n");
				fprintf(stderr, "...Total entries: %d", dwTotalEntries);
			}

			printf("\nEntries enumerated: %d\n", dwTotalCount);

		} else {
			printf("...No servers were found\n");
			printf("...The buffer (bufptr) returned was nullptr\n");
			printf("...entriesread: %d\n", dwEntriesRead);
			printf("...totalentries: %d\n", dwEntriesRead);
		}

	} else {
		fprintf(stderr, "...NetServerEnum failed with error: %d\n", nStatus);
	}
	
	// Free the allocated buffer.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
}

//The NetWkstaGetInfo function returns information about the configuration of a workstation.
void LOLANetUse::NetWkstaGetInfoRunner(int argc, wchar_t *argv[])
{
	DWORD dwLevel = 102;
	LPWKSTA_INFO_102 pBuf = nullptr;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = nullptr;

	// Check command line arguments.
	if (argc > 2) {
		fwprintf(stderr, L"...Usage: %s [\\\\ServerName]\n", argv[0]);
		exit(1);
	}
	// The server is not the default local computer.
	if (argc == 2){
		pszServerName = argv[1];
	}
	// Call the NetWkstaGetInfo function, specifying level 102.
	nStatus = NetWkstaGetInfo(pszServerName, dwLevel, (LPBYTE *)&pBuf);

	// If the call is successful, print the workstation data.
	if (nStatus == NERR_Success){
		printf("\n\tPlatform: %d\n", pBuf->wki102_platform_id);
		wprintf(L"\tName:     %ls\n", pBuf->wki102_computername);
		printf("\tVersion:  %d.%d\n", pBuf->wki102_ver_major, pBuf->wki102_ver_minor);
		wprintf(L"\tDomain:   %ls\n", pBuf->wki102_langroup);
		wprintf(L"\tLan Root: %ls\n", pBuf->wki102_lanroot);
		wprintf(L"\t# Logged On Users: %d\n", pBuf->wki102_logged_on_users);
	} else { // Otherwise, indicate the system error.
		fprintf(stderr, "...A system error has occurred: %d\n", nStatus);
	}
	// Free the allocated memory.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
}

//The NetRemoteComputerSupports function queries the redirector to retrieve the optional features the remote system supports. 
//Features include Unicode, Remote Procedure Call (RPC), and Remote Administration Protocol support. 
//The function establishes a network connection if one does not exist.
void LOLANetUse::NetRemoteComputerSupportsRunner(int argc, wchar_t *argv[])
{
	NET_API_STATUS nStatus;
	LPCWSTR UncServerName = nullptr;
	DWORD OptionsWanted = SUPPORTS_LOCAL;
	LPDWORD OptionsSupported = nullptr;

	// Check command line arguments.
	if (argc > 2){
		fwprintf(stderr, L"...Usage: %s [\\\\ServerName]\n", argv[0]);
		exit(1);
	}
	// The server is not the default local computer.
	if (argc == 2){
		UncServerName = argv[1];
	}

	nStatus = NetRemoteComputerSupports(UncServerName, OptionsWanted, (DWORD *)&OptionsSupported);
	if (nStatus == NERR_Success){
		if (OptionsSupported == nullptr){ 
			printf("~~~Options supported is set to nullptr"); 
		}
		printf("\n\tOptions Supported: %d\n", *	OptionsSupported);
	} else {
		fprintf(stderr, "...A system error has occurred: %d\n", nStatus);
	}
	// Free the allocated memory.
	if (OptionsSupported != nullptr){
		NetApiBufferFree(OptionsSupported);
	}

}

//The NetUserEnum function retrieves information about all user accounts on a server.
int LOLANetUse::NetUserEnumRunner(int argc, wchar_t *argv[])
{
	LPUSER_INFO_0 pBuf = nullptr;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = nullptr;

	if (argc > 2){
		fwprintf(stderr, L"...Usage: %s [\\\\ServerName]\n", argv[0]);
		exit(1);
	}
	//The server is not the default local computer.
	if (argc == 2){
		pszServerName = argv[1];
	}
	wprintf(L"\n...User accounts on %ls: \n", pszServerName);
	//Call the NetUserEnum function, specifying level 0; enumerate global user account types only.
	do{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
		dwLevel,
		FILTER_NORMAL_ACCOUNT, // global users
		(LPBYTE*)&pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries,
		&dwResumeHandle);
		//If the call succeeds,
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
			if ((pTmpBuf = pBuf) != nullptr){
				//Loop through the entries.
				for (i = 0; (i < dwEntriesRead); i++){
					assert(pTmpBuf != nullptr);

					if (pTmpBuf == nullptr){
						fprintf(stderr, "...An access violation has occurred\n");
						break;
					}
					//Print the name of the user account.
					wprintf(L"\t--%ls\n", pTmpBuf->usri0_name);

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		} else {
		//Otherwise, print the system error.
			fprintf(stderr, "...A system error has occurred: %d\n", nStatus);
		}
		//Free the allocated buffer.
		if (pBuf != nullptr){
			NetApiBufferFree(pBuf);
			pBuf = nullptr;
		}
	}
	//Continue to call NetUserEnum while there are more entries. 
	while (nStatus == ERROR_MORE_DATA); // end do
	//Check again for allocated memory.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
	//Print the final count of users enumerated.
	fprintf(stderr, "\n...Total of %d entries enumerated\n", dwTotalCount);

	return 0;
}

//The NetUserGetInfo function retrieves information about a particular user account on a server.
int LOLANetUse::NetUserGetInfoRunner(int argc, wchar_t *argv[])
{
	DWORD dwLevel = 0;

	LPUSER_INFO_0 pBuf = nullptr;
	LPUSER_INFO_1 pBuf1 = nullptr;
	LPUSER_INFO_2 pBuf2 = nullptr;
	//LPUSER_INFO_3 pBuf3 = nullptr;
	LPUSER_INFO_4 pBuf4 = nullptr;
	LPUSER_INFO_10 pBuf10 = nullptr;
	LPUSER_INFO_11 pBuf11 = nullptr;
	LPUSER_INFO_20 pBuf20 = nullptr;
	LPUSER_INFO_23 pBuf23 = nullptr;

	NET_API_STATUS nStatus;

	LPTSTR sStringSid = nullptr;

	int i = 0;
	int j = 0;

	if (argc != 3){
		fwprintf(stderr, L"...Usage: %s \\\\ServerName UserName\n", argv[0]);
		exit(1);
	}

	while (i < 24){
		// Call the NetUserGetInfo function.
		dwLevel = i;
		wprintf(L"\n...Calling NetUserGetinfo with Servername=%s Username=%s Level=%d\n",
			argv[1], argv[2], dwLevel);
		nStatus = NetUserGetInfo(argv[1], argv[2], dwLevel, (LPBYTE *)& pBuf);
		// If the call succeeds, print the user information.
		if (nStatus == NERR_Success){
			if (pBuf != nullptr){
				switch (i){
					case 0:
						wprintf(L"\tUser account name: %s\n", pBuf->usri0_name);
						break;
					case 1:
						pBuf1 = (LPUSER_INFO_1)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf1->usri1_name);
						wprintf(L"\tPassword: %s\n", pBuf1->usri1_password);
						wprintf(L"\tPassword age (seconds): %d,\n",pBuf1->usri1_password_age);
						wprintf(L"\tPassword age (hours): %d,\n",pBuf1->usri1_password_age / 60 / 60);
						wprintf(L"\tPassword age (days): %d,\n",pBuf1->usri1_password_age / 60 / 60 / 24);
						wprintf(L"\tPrivilege level: %d\n", pBuf1->usri1_priv);
						wprintf(L"\tHome directory: %s\n", pBuf1->usri1_home_dir);
						wprintf(L"\tUser comment: %s\n", pBuf1->usri1_comment);
						wprintf(L"\tFlags (in hex): %x\n", pBuf1->usri1_flags);
						wprintf(L"\tScript path: %s\n", pBuf1->usri1_script_path);
						break;
					case 2:
						pBuf2 = (LPUSER_INFO_2)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf2->usri2_name);
						wprintf(L"\tPassword: %s\n", pBuf2->usri2_password);
						wprintf(L"\tPassword age (seconds): %d\n",pBuf2->usri2_password_age);
						wprintf(L"\tPassword age (hours): %d,\n",pBuf2->usri2_password_age / 60 / 60);
						wprintf(L"\tPassword age (days): %d,\n",pBuf2->usri2_password_age / 60 / 60 / 24);
						wprintf(L"\tPrivilege level: %d\n", pBuf2->usri2_priv);
						wprintf(L"\tHome directory: %s\n", pBuf2->usri2_home_dir);
						wprintf(L"\tComment: %s\n", pBuf2->usri2_comment);
						wprintf(L"\tFlags (in hex): %x\n", pBuf2->usri2_flags);
						wprintf(L"\tScript path: %s\n", pBuf2->usri2_script_path);
						wprintf(L"\tAuth flags (in hex): %x\n",pBuf2->usri2_auth_flags);
						wprintf(L"\tFull name: %s\n", pBuf2->usri2_full_name);
						wprintf(L"\tUser comment: %s\n", pBuf2->usri2_usr_comment);
						wprintf(L"\tParameters: %s\n", pBuf2->usri2_parms);
						wprintf(L"\tWorkstations: %s\n", pBuf2->usri2_workstations);
						wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",
							pBuf2->usri2_last_logon);
						wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",
							pBuf2->usri2_last_logoff);
						wprintf(L"\tAccount expires (seconds since January 1, 1970 GMT): %d\n",
							pBuf2->usri2_acct_expires);
						wprintf(L"\tMax storage: %d\n", pBuf2->usri2_max_storage);
						wprintf(L"\tUnits per week: %d\n",pBuf2->usri2_units_per_week);
						wprintf(L"\tLogon hours:");
						for (j = 0; j < 21; j++){
							printf(" %x", pBuf2->usri2_logon_hours[j]);
						}
						wprintf(L"\n");
						wprintf(L"\tBad password count: %d\n",pBuf2->usri2_bad_pw_count);
						wprintf(L"\tNumber of logons: %d\n",pBuf2->usri2_num_logons);
						wprintf(L"\tLogon server: %s\n", pBuf2->usri2_logon_server);
						wprintf(L"\tCountry code: %d\n", pBuf2->usri2_country_code);
						wprintf(L"\tCode page: %d\n", pBuf2->usri2_code_page);
						break;
					case 4:
						pBuf4 = (LPUSER_INFO_4)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf4->usri4_name);
						wprintf(L"\tPassword: %s\n", pBuf4->usri4_password);
						wprintf(L"\tPassword age (seconds): %d\n",pBuf4->usri4_password_age);
						wprintf(L"\tPassword age (hours): %d\n",pBuf4->usri4_password_age / 60 / 60);
						wprintf(L"\tPassword age (days): %d\n",pBuf4->usri4_password_age / 60 / 60 / 24);
						wprintf(L"\tPrivilege level: %d\n", pBuf4->usri4_priv);
						wprintf(L"\tHome directory: %s\n", pBuf4->usri4_home_dir);
						wprintf(L"\tComment: %s\n", pBuf4->usri4_comment);
						wprintf(L"\tFlags (in hex): %x\n", pBuf4->usri4_flags);
						wprintf(L"\tScript path: %s\n", pBuf4->usri4_script_path);
						wprintf(L"\tAuth flags (in hex): %x\n",pBuf4->usri4_auth_flags);
						wprintf(L"\tFull name: %s\n", pBuf4->usri4_full_name);
						wprintf(L"\tUser comment: %s\n", pBuf4->usri4_usr_comment);
						wprintf(L"\tParameters: %s\n", pBuf4->usri4_parms);
						wprintf(L"\tWorkstations: %s\n", pBuf4->usri4_workstations);
						wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",
							pBuf4->usri4_last_logon);
						wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",
							pBuf4->usri4_last_logoff);
						wprintf(L"\tAccount expires (seconds since January 1, 1970 GMT): %d\n",
							pBuf4->usri4_acct_expires);
						wprintf(L"\tMax storage: %d\n", pBuf4->usri4_max_storage);
						wprintf(L"\tUnits per week: %d\n",pBuf4->usri4_units_per_week);
						wprintf(L"\tLogon hours:");
						for (j = 0; j < 21; j++){
							printf(" %x", pBuf4->usri4_logon_hours[j]);
						}
						wprintf(L"\n");
						wprintf(L"\tBad password count: %d\n",pBuf4->usri4_bad_pw_count);
						wprintf(L"\tNumber of logons: %d\n",pBuf4->usri4_num_logons);
						wprintf(L"\tLogon server: %s\n", pBuf4->usri4_logon_server);
						wprintf(L"\tCountry code: %d\n", pBuf4->usri4_country_code);
						wprintf(L"\tCode page: %d\n", pBuf4->usri4_code_page);
						if (ConvertSidToStringSid(pBuf4->usri4_user_sid, &sStringSid)){
							wprintf(L"\tUser SID: %s\n", sStringSid);
							LocalFree(sStringSid);
						} else {
							wprintf(L"ConvertSidToSTringSid failed with error %d\n",GetLastError());
						}
						wprintf(L"\tPrimary group ID: %d\n",pBuf4->usri4_primary_group_id);
						wprintf(L"\tProfile: %s\n", pBuf4->usri4_profile);
						wprintf(L"\tHome directory drive letter: %s\n",pBuf4->usri4_home_dir_drive);
						wprintf(L"\tPassword expired information: %d\n",pBuf4->usri4_password_expired);
						break;
					case 10:
						pBuf10 = (LPUSER_INFO_10)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf10->usri10_name);
						wprintf(L"\tComment: %s\n", pBuf10->usri10_comment);
						wprintf(L"\tUser comment: %s\n",pBuf10->usri10_usr_comment);
						wprintf(L"\tFull name: %s\n", pBuf10->usri10_full_name);
						break;
					case 11:
						pBuf11 = (LPUSER_INFO_11)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf11->usri11_name);
						wprintf(L"\tComment: %s\n", pBuf11->usri11_comment);
						wprintf(L"\tUser comment: %s\n",pBuf11->usri11_usr_comment);
						wprintf(L"\tFull name: %s\n", pBuf11->usri11_full_name);
						wprintf(L"\tPrivilege level: %d\n", pBuf11->usri11_priv);
						wprintf(L"\tAuth flags (in hex): %x\n",pBuf11->usri11_auth_flags);
						wprintf(L"\tPassword age (seconds): %d\n",pBuf11->usri11_password_age);
						wprintf(L"\tPassword age (hours): %d\n",pBuf11->usri11_password_age / 60 / 60);
						wprintf(L"\tPassword age (days): %d\n",pBuf11->usri11_password_age / 60 / 60 / 24);
						wprintf(L"\tHome directory: %s\n", pBuf11->usri11_home_dir);
						wprintf(L"\tParameters: %s\n", pBuf11->usri11_parms);
						wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",
							pBuf11->usri11_last_logon);
						wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",
							pBuf11->usri11_last_logoff);
						wprintf(L"\tBad password count: %d\n",pBuf11->usri11_bad_pw_count);
						wprintf(L"\tNumber of logons: %d\n",pBuf11->usri11_num_logons);
						wprintf(L"\tLogon server: %s\n",pBuf11->usri11_logon_server);
						wprintf(L"\tCountry code: %d\n",pBuf11->usri11_country_code);
						wprintf(L"\tWorkstations: %s\n",pBuf11->usri11_workstations);
						wprintf(L"\tMax storage: %d\n", pBuf11->usri11_max_storage);
						wprintf(L"\tUnits per week: %d\n",pBuf11->usri11_units_per_week);
						wprintf(L"\tLogon hours:");
						for (j = 0; j < 21; j++){
							printf(" %x", pBuf11->usri11_logon_hours[j]);
						}
						wprintf(L"\n");
						wprintf(L"\tCode page: %d\n", pBuf11->usri11_code_page);
						break;
					case 20:
						pBuf20 = (LPUSER_INFO_20)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf20->usri20_name);
						wprintf(L"\tFull name: %s\n", pBuf20->usri20_full_name);
						wprintf(L"\tComment: %s\n", pBuf20->usri20_comment);
						wprintf(L"\tFlags (in hex): %x\n", pBuf20->usri20_flags);
						wprintf(L"\tUser ID: %u\n", pBuf20->usri20_user_id);
						break;
					case 23:
						pBuf23 = (LPUSER_INFO_23)pBuf;
						wprintf(L"\tUser account name: %s\n", pBuf23->usri23_name);
						wprintf(L"\tFull name: %s\n", pBuf23->usri23_full_name);
						wprintf(L"\tComment: %s\n", pBuf23->usri23_comment);
						wprintf(L"\tFlags (in hex): %x\n", pBuf23->usri23_flags);
						if (ConvertSidToStringSid(pBuf23->usri23_user_sid, &sStringSid)){
							wprintf(L"\tUser SID: %s\n", sStringSid);
							LocalFree(sStringSid);
						} else {
							wprintf(L"ConvertSidToSTringSid failed with error %d\n",
							GetLastError());
						}
						break;
					default:
						break;
				}
			}
		} else { // Otherwise, print the system error.
			fprintf(stderr, "...NetUserGetinfo failed with error: %d\n", nStatus);
		}
		// Free the allocated memory.
		if (pBuf != nullptr){
			NetApiBufferFree(pBuf);
		}
		switch (i){
			case 0:
			case 1:
			case 10:
				i++;
				break;
			case 2:
				i = 4;
				break;
			case 4:
				i = 10;
				break;
			case 11:
				i = 20;
				break;
			case 20:
				i = 23;
				break;
			default:
				i = 24;
				break;
		}
	}
	return 0;
}

//The NetUserModalsGet function retrieves global information for all users and global groups in the security database, 
//which is the security accounts manager(SAM) database or, in the case of domain controllers, the Active Directory.
int LOLANetUse::NetUserModalsGetRunner(int argc, wchar_t *argv[])
{
	DWORD dwLevel = 0;
	USER_MODALS_INFO_0 *pBuf = nullptr;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = nullptr;

	if (argc > 2){
		fwprintf(stderr, L"...Usage: %s [\\\\ServerName]\n", argv[0]);
		exit(1);
	}
	// The server is not the default local computer.
	if (argc == 2){
		pszServerName = argv[1];
	}
	// Call the NetUserModalsGet function; specify level 0.
	nStatus = NetUserModalsGet((LPCWSTR)pszServerName,dwLevel,(LPBYTE *)&pBuf);
	// If the call succeeds, print the global information.
	if (nStatus == NERR_Success){
		if (pBuf != nullptr){
			printf("\tMinimum password length:  %d\n", pBuf->usrmod0_min_passwd_len);
			printf("\tMaximum password age (d): %d\n", pBuf->usrmod0_max_passwd_age / 86400);
			printf("\tMinimum password age (d): %d\n", pBuf->usrmod0_min_passwd_age / 86400);
			printf("\tForced log off time (s):  %d\n", pBuf->usrmod0_force_logoff);
			printf("\tPassword history length:  %d\n", pBuf->usrmod0_password_hist_len);
		}
	} else { // Otherwise, print the system error.
		fprintf(stderr, "...A system error has occurred: %d\n", nStatus);
	}
	// Free the allocated memory.
	if (pBuf != nullptr){
		NetApiBufferFree(pBuf);
	}
	return 0;
}

//The NetQueryDisplayInformation function returns user account, computer, or group account information.
int LOLANetUse::NetQueryDisplayInformationRunner(wchar_t *argv[], int level)
{
	PNET_DISPLAY_GROUP pBuff, p;
	DWORD res, dwRec, i = 0;

	do { // begin do
		//Call the NetQueryDisplayInformation function; specify information level 3 (group account information).
		res = NetQueryDisplayInformation(argv[1], level, i, 100, MAX_PREFERRED_LENGTH, &dwRec, (PVOID*)&pBuff);
		// If the call succeeds,
		if ((res == ERROR_SUCCESS) || (res == ERROR_MORE_DATA)){
			p = pBuff;
			for (; dwRec>0; dwRec--){
				// Print the retrieved group information.
				printf("\t Name:      %S\n"
					   "\t Comment:   %S\n"
					   "\t Group ID:  %u\n"
					   "\t Attributes: %u\n"
					   "\t --------------------------------\n",
					p->grpi3_name, p->grpi3_comment, p->grpi3_group_id, p->grpi3_attributes);
				// If there is more data, set the index.
				i = p->grpi3_next_index;
				p++;
			}
			// Free the allocated memory.
			NetApiBufferFree(pBuff);
		} else {
			printf("...Error: %u\n", res);
		}
		// Continue while there is more data.
	} while (res == ERROR_MORE_DATA); // end do
	return 0;
}
