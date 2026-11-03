# DE10

Included are all of the assingment required files, including the `.qpf`, `.rbf`, `.dtbo`, and `.c`.

The `DE10_Standard_GHRD.qpf` file is the Quartus Prime file. It was initially gotten from the DE10_Standard_GHRD folder from Terasic, opened in Quartus Prime, then modified. 

The `soc_system.rbf` file was created in Platform Designer inside Quartus Prime, then uploaded to the board. 

The `soc_sustem.dtbo` was then created on the DE10 board with the `soc_system.rbf` file, and retrieved off the board for upload here. 

The `memory_mapping_showcase.c` file was created to show all memory mappings are functional. It showcases the 7-segment displays, which were the new mappings, as well as the LEDs, Slide Switches, and Push Buttons that were already mapped in the default file. 

The DE10_Standard_GHRD folder is the entire project folder that was downloaded from Terasic and initally opened in Quartus Prime, should there be anything in that folder that is needed that is not in the Required Files folder. 