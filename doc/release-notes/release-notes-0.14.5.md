Commercium version 0.14.5 is now available from:

  <https://download.commercium.org/0.14.5/>

This release includes the following features and fixes:

- Enforce strong replay protection (require SIGHASH_FORKID
  and SCRIPT_VERIFY_STRICTENC compliance)
- Change currency units in user interface from CMM -> CMM
- Add NODE_COMMERCIUM_CASH service bit (0x20)
- Update BU backed seeder to btncash-seeder.commerciumunlimited.info
- Update ABC logos for About menu, and testnet icon 
- Various refactoring and cleanups

NOTE: Change in default value:
- 'forcednsseed' is enabled by default to increase
  chance of good connection to UAHF network (see D360)
