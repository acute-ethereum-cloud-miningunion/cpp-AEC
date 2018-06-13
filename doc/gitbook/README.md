cpp-aecmio guide
======= 

This book is intended as a practical user guide for the cpp-aecmio software distribution.

cpp-aecmio is a distribution of software including a number of diverse tools. This book begins with the installation instructions, before proceeding to introductions, walk-throughs and references for the various tools that make up cpp-aecmio.

The full software suite of cpp-aecmio includes:

- **aec** (`aec`) The mainline CLI aecmio client. Run it in the background and it will connect to the aecmio network; you can mine, make transactions and inspect the blockchain.
- `aeckey` A key/wallet management tool for aecmio keys. This lets you add, remove and change your keys as well as *cold wallet device*-friendly transaction inspection and signing.
- `aecminer` A standalone miner. This can be used to check how fast you can mine and will mine for you in concert with `aec`, `gaec` and `pyaecmio`.
- `aecvm` The aecmio virtual machine emulator. You can use this to run EVM code.
- `rlp` An serialisation/deserialisation tool for the Recursive Length Prefix format.
