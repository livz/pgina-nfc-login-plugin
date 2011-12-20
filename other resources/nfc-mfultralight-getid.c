/**
 * Based on
 * @file nfc-mfultralight.c
 * @brief MIFARE Ultralight id tool
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif // HAVE_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "nfc-utils.h"
#include "mifare.h"

static nfc_device_t *pnd;
static nfc_target_t nt;
static mifareul_tag mtDump;

static const nfc_modulation_t nmMifare = {
  .nmt = NMT_ISO14443A,
  .nbr = NBR_106,
};

int
main (int argc, const char *argv[])
{
	int uid = 0;
	memset (&mtDump, 0x00, sizeof (mtDump));

	// Try to open the NFC device
	pnd = nfc_connect (NULL);
	if (pnd == NULL) {
		ERR ("Error connecting NFC device\n");
		return 11;
	}

  nfc_initiator_init (pnd);

  // Let the device only try once to find a tag
  if (!nfc_configure (pnd, NDO_INFINITE_SELECT, false)) {
    nfc_perror (pnd, "nfc_configure");
    return 12;
  }

  printf ("Connected to NFC device: %s\n", pnd->acName);

  // Try to find a MIFARE Ultralight tag
  if (!nfc_initiator_select_passive_target (pnd, nmMifare, NULL, 0, &nt)) {
    ERR ("no tag was found\n");
    nfc_disconnect (pnd);
    return 13;
  }
  // Test if we are dealing with a MIFARE compatible tag

  if (nt.nti.nai.abtAtqa[1] != 0x44) {
    ERR ("tag is not a MIFARE Ultralight card\n");
    nfc_disconnect (pnd);
    return 14;
  }
  // Get the info from the current tag
  printf ("Found MIFARE Ultralight card with UID (len: %d): ", nt.nti.nai.szUidLen);
  size_t  szPos;
  for (szPos = 0; szPos < nt.nti.nai.szUidLen; szPos++) {
    printf ("%02x", nt.nti.nai.abtUid[szPos]);
  }

  nfc_disconnect (pnd);

  uid = *((int *)nt.nti.nai.abtUid);
  printf("\nConverted UID: %d\n", uid);

  return uid;
}
