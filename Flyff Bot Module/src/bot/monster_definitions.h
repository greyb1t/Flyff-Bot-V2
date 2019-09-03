#pragma once

#include <string>
#include <map>
#include <Windows.h>

static std::map<int32_t, std::wstring> g_monster_names = {
  { 10, TEXT( "Default" ) },
  { 11, TEXT( "Male" ) },
  { 12, TEXT( "Female" ) },
  { 20, TEXT( "Small Aibatt" ) },
  { 21, TEXT( "Aibatt" ) },
  { 22, TEXT( "Captain Aibatt" ) },
  { 23, TEXT( "Giant Aibatt" ) },
  { 24, TEXT( "Small Burudeng" ) },
  { 25, TEXT( "Burudeng" ) },
  { 26, TEXT( "Captain Burudeng" ) },
  { 27, TEXT( "Giant Burudeng" ) },
  { 28, TEXT( "Small Pukepuke" ) },
  { 29, TEXT( "Pukepuke" ) },
  { 30, TEXT( "Captain Pukepuke" ) },
  { 31, TEXT( "Giant Pukepuke" ) },
  { 32, TEXT( "Small Doridoma" ) },
  { 33, TEXT( "Doridoma" ) },
  { 34, TEXT( "Captain Doridoma" ) },
  { 35, TEXT( "Giant Doridoma" ) },
  { 36, TEXT( "Young Lawolf" ) },
  { 37, TEXT( "Lawolf" ) },
  { 38, TEXT( "Grownup Lawolf" ) },
  { 39, TEXT( "Giant Lawolf" ) },
  { 40, TEXT( "Cute Nyangnyang" ) },
  { 41, TEXT( "Nyangnyang" ) },
  { 42, TEXT( "Creep Nyangnyang" ) },
  { 43, TEXT( "Giant Nyangnyang" ) },
  { 44, TEXT( "Bang" ) },
  { 45, TEXT( "Captain Bang" ) },
  { 46, TEXT( "Boss Bang" ) },
  { 47, TEXT( "Giant Bang" ) },
  { 48, TEXT( "Small Wagsaac" ) },
  { 49, TEXT( "Wagsaac" ) },
  { 50, TEXT( "Captain Wagsaac" ) },
  { 51, TEXT( "Giant Wagsaac" ) },
  { 52, TEXT( "Rough Wheelem" ) },
  { 53, TEXT( "Wheelem" ) },
  { 54, TEXT( "Bouncing Wheelem" ) },
  { 55, TEXT( "Giant Wheelem" ) },
  { 56, TEXT( "Small Tombstone Bearer" ) },
  { 57, TEXT( "Tombstone Bearer" ) },
  { 58, TEXT( "Captain Tombstone Bearer" ) },
  { 59, TEXT( "Giant Tombstone Bearer" ) },
  { 60, TEXT( "1st Class Fefern" ) },
  { 61, TEXT( "Fefern" ) },
  { 62, TEXT( "2nd Class Fefern" ) },
  { 63, TEXT( "Giant Fefern" ) },
  { 64, TEXT( "Red Mantis C" ) },
  { 65, TEXT( "Red Mantis B" ) },
  { 66, TEXT( "Red Mantis A" ) },
  { 67, TEXT( "Giant Red Mantis" ) },
  { 68, TEXT( "Stale Mr. Pumpkin" ) },
  { 69, TEXT( "Mr. Pumpkin" ) },
  { 70, TEXT( "Rotten Mr Pumpkin" ) },
  { 71, TEXT( "Giant Mr. Pumpkin" ) },
  { 72, TEXT( "Phantom Basque" ) },
  { 73, TEXT( "Basque" ) },
  { 74, TEXT( "Lumping Basque" ) },
  { 75, TEXT( "Giant Basque" ) },
  { 80, TEXT( "Small Mage Prankster" ) },
  { 81, TEXT( "Prankster" ) },
  { 82, TEXT( "Master Mage Prankster" ) },
  { 83, TEXT( "Giantmage Prankster" ) },
  { 84, TEXT( "Maid Cardpuppet" ) },
  { 85, TEXT( "Cardpuppet" ) },
  { 86, TEXT( "Manager Cardpuppet" ) },
  { 87, TEXT( "Boss Cardpuppet" ) },
  { 88, TEXT( "Urchin Demian" ) },
  { 89, TEXT( "Demian" ) },
  { 90, TEXT( "Bad Demian" ) },
  { 91, TEXT( "Giant Demian" ) },
  { 92, TEXT( "Small Rock Muscle" ) },
  { 93, TEXT( "Rock Muscle" ) },
  { 94, TEXT( "Captain Rock Muscle" ) },
  { 95, TEXT( "Giant Rock Muscle" ) },
  { 96, TEXT( "Small Mushpang" ) },
  { 97, TEXT( "Mushpang" ) },
  { 98, TEXT( "Captain Mushpang" ) },
  { 99, TEXT( "Giant Mushpang" ) },
  { 100, TEXT( "Brazenface Pukepuke" ) },
  { 101, TEXT( "Heartless Bang" ) },
  { 102, TEXT( "Tough Rock Muscle" ) },
  { 103, TEXT( "Small Totemia" ) },
  { 104, TEXT( "Totemia" ) },
  { 105, TEXT( "Captain Totemia" ) },
  { 106, TEXT( "Giant Totemia" ) },
  { 107, TEXT( "Small Steam Walker" ) },
  { 108, TEXT( "Steam Walker" ) },
  { 109, TEXT( "Captain Steam Walker" ) },
  { 110, TEXT( "Giant Steam Walker" ) },
  { 111, TEXT( "Flying Aibatt" ) },
  { 112, TEXT( "Small Volt" ) },
  { 113, TEXT( "Volt" ) },
  { 114, TEXT( "Captain Volt" ) },
  { 115, TEXT( "Giant Volt" ) },
  { 116, TEXT( "Small Circus Bear" ) },
  { 117, TEXT( "Circus Bear" ) },
  { 118, TEXT( "Captain Circus Bear" ) },
  { 119, TEXT( "Giant Circus Bear" ) },
  { 120, TEXT( "Small Mia" ) },
  { 121, TEXT( "Mia" ) },
  { 122, TEXT( "Captain Mia" ) },
  { 123, TEXT( "Giant Mia" ) },
  { 130, TEXT( "Small Pire" ) },
  { 131, TEXT( "Pire" ) },
  { 132, TEXT( "Captain Pire" ) },
  { 133, TEXT( "Giant Pire" ) },
  { 134, TEXT( "Small Froziest" ) },
  { 135, TEXT( "Froziest" ) },
  { 136, TEXT( "Captain Froziest" ) },
  { 137, TEXT( "Giant Froziest" ) },
  { 160, TEXT( "Small Burn Bird" ) },
  { 161, TEXT( "Burn Bird" ) },
  { 162, TEXT( "Captain Burn Bird" ) },
  { 163, TEXT( "Giant Burn Bird" ) },
  { 164, TEXT( "Clockworks" ) },
  { 165, TEXT( "Big Muscle" ) },
  { 166, TEXT( "Krrr" ) },
  { 167, TEXT( "Mushmoot" ) },
  { 168, TEXT( "Small Mushpoie" ) },
  { 190, TEXT( "Small Buketfoot" ) },
  { 191, TEXT( "Buketfoot" ) },
  { 192, TEXT( "Captain Buketfoot" ) },
  { 193, TEXT( "Giant Buketfoot" ) },
  { 196, TEXT( "Master Demian" ) },
  { 197, TEXT( "Master Keakoon" ) },
  { 198, TEXT( "Master Muffrin" ) },
  { 199, TEXT( "Beast Mr. Pumpkin" ) },
  { 200, TEXT( "Infopeng" ) },
  { 201, TEXT( "Dwarpet" ) },
  { 202, TEXT( "Master Dwarpet" ) },
  { 203, TEXT( "Iblis" ) },
  { 204, TEXT( "Shade" ) },
  { 205, TEXT( "Bubble" ) },
  { 206, TEXT( "Beast Zombiger" ) },
  { 210, TEXT( "Losha" ) },
  { 211, TEXT( "Boboku" ) },
  { 212, TEXT( "Julia" ) },
  { 213, TEXT( "Lui" ) },
  { 214, TEXT( "Marche" ) },
  { 215, TEXT( "Bulrox" ) },
  { 216, TEXT( "Tina" ) },
  { 217, TEXT( "Karin" ) },
  { 218, TEXT( "Martin" ) },
  { 219, TEXT( "Bill" ) },
  { 220, TEXT( "Rhine" ) },
  { 221, TEXT( "Stima" ) },
  { 222, TEXT( "Phacham" ) },
  { 223, TEXT( "Roocky" ) },
  { 224, TEXT( "Ollien" ) },
  { 225, TEXT( "Haven" ) },
  { 226, TEXT( "Chitller" ) },
  { 227, TEXT( "Bolpor" ) },
  { 228, TEXT( "Almani" ) },
  { 229, TEXT( "Achaben" ) },
  { 230, TEXT( "Eshylop" ) },
  { 231, TEXT( "Remine" ) },
  { 232, TEXT( "Unknown" ) },
  { 233, TEXT( "Agent" ) },
  { 234, TEXT( "Guardian" ) },
  { 235, TEXT( "Ancimys" ) },
  { 236, TEXT( "Santa" ) },
  { 237, TEXT( "Priest" ) },
  { 238, TEXT( "Chitos" ) },
  { 239, TEXT( "Polar Bear" ) },
  { 300, TEXT( "Small Flbyrigen" ) },
  { 301, TEXT( "Flbyrigen" ) },
  { 302, TEXT( "Captain Flbyrigen" ) },
  { 303, TEXT( "Giant Flbyrigen" ) },
  { 304, TEXT( "Small Mothbee" ) },
  { 305, TEXT( "Mothbee" ) },
  { 306, TEXT( "Captain Mothbee" ) },
  { 307, TEXT( "Giant Mothbee" ) },
  { 308, TEXT( "Small Rockepeller" ) },
  { 309, TEXT( "Rockepeller" ) },
  { 310, TEXT( "Captain Rockepeller" ) },
  { 311, TEXT( "Giant Rockepeller" ) },
  { 400, TEXT( "Small Giggle Box" ) },
  { 401, TEXT( "Giggle Box" ) },
  { 402, TEXT( "Captain Giggle Box" ) },
  { 403, TEXT( "Giant Giggle Box" ) },
  { 404, TEXT( "Small Garbagepider" ) },
  { 405, TEXT( "Garbagepider" ) },
  { 406, TEXT( "Captain Garbagepider" ) },
  { 407, TEXT( "Giant Garbagepider" ) },
  { 408, TEXT( "Small Rampage Butcher" ) },
  { 409, TEXT( "Small Rampage Butcher" ) },
  { 410, TEXT( "Captain Rampage Butcher" ) },
  { 411, TEXT( "Giant Rampage Butcher" ) },
  { 500, TEXT( "Small Greemong" ) },
  { 501, TEXT( "Greemong" ) },
  { 502, TEXT( "Captain Greemong" ) },
  { 503, TEXT( "Giant Greemong" ) },
  { 504, TEXT( "Small Driller" ) },
  { 505, TEXT( "Driller" ) },
  { 506, TEXT( "Captain Driller" ) },
  { 507, TEXT( "Giant Driller" ) },
  { 508, TEXT( "Small Steel Knight" ) },
  { 509, TEXT( "Steel Knight" ) },
  { 510, TEXT( "Captain Steel Knight" ) },
  { 511, TEXT( "Giant Steel Knight" ) },
  { 512, TEXT( "Small Elderguard" ) },
  { 513, TEXT( "Elderguard" ) },
  { 514, TEXT( "Captain Elderguard" ) },
  { 515, TEXT( "Giant Elderguard" ) },
  { 516, TEXT( "Small Crane Machinery" ) },
  { 517, TEXT( "Crane Machinery" ) },
  { 518, TEXT( "Captain Crane Machinery" ) },
  { 519, TEXT( "Giant Crane Machinery" ) },
  { 520, TEXT( "Pawn Popcrank" ) },
  { 521, TEXT( "Knight Popcrank" ) },
  { 522, TEXT( "Rook Popcrank" ) },
  { 523, TEXT( "Bishop Popcrank" ) },
  { 524, TEXT( "Small Peakyturtle" ) },
  { 525, TEXT( "Peakyturtle" ) },
  { 526, TEXT( "Captain Peakyturtle" ) },
  { 527, TEXT( "Giant Peakyturtle" ) },
  { 528, TEXT( "Small Hobo" ) },
  { 529, TEXT( "Hobo" ) },
  { 530, TEXT( "Captain Hobo" ) },
  { 531, TEXT( "Giant Hobo" ) },
  { 532, TEXT( "Small Clawdoll" ) },
  { 533, TEXT( "Clawdoll" ) },
  { 534, TEXT( "Captain Clawdoll" ) },
  { 535, TEXT( "Giant Clawdoll" ) },
  { 536, TEXT( "Small Carrierbomb" ) },
  { 537, TEXT( "Carrierbomb" ) },
  { 538, TEXT( "Captain Carrierbomb" ) },
  { 539, TEXT( "Giant Carrierbomb" ) },
  { 540, TEXT( "Small Leyena" ) },
  { 541, TEXT( "Leyena" ) },
  { 542, TEXT( "Captain Leyena" ) },
  { 543, TEXT( "Giant Leyena" ) },
  { 544, TEXT( "Small Dumb Bull" ) },
  { 545, TEXT( "Dumb Bull" ) },
  { 546, TEXT( "Captain Dumb Bull" ) },
  { 547, TEXT( "Giant Dumb Bull" ) },
  { 548, TEXT( "Small Nutty Wheel" ) },
  { 549, TEXT( "Nutty Wheel" ) },
  { 550, TEXT( "Captain Nutty Wheel" ) },
  { 551, TEXT( "Giant Nutty Wheel" ) },
  { 552, TEXT( "Small Jack the Hammer" ) },
  { 553, TEXT( "Jack the Hammer" ) },
  { 554, TEXT( "Captain Jack the Hammer" ) },
  { 555, TEXT( "Giant Jack the Hammer" ) },
  { 556, TEXT( "Mine Catcher" ) },
  { 557, TEXT( "Erons Catcher" ) },
  { 558, TEXT( "Krasec Catcher" ) },
  { 559, TEXT( "Guru Catcher" ) },
  { 560, TEXT( "Small Nuctuvehicle" ) },
  { 561, TEXT( "Nuctuvehicle" ) },
  { 562, TEXT( "Captain Nuctuvehicle" ) },
  { 563, TEXT( "Giant Nuctuvehicle" ) },
  { 564, TEXT( "Small Risem" ) },
  { 565, TEXT( "Risem" ) },
  { 566, TEXT( "Captain Risem" ) },
  { 567, TEXT( "Giant Risem" ) },
  { 568, TEXT( "Young Syliaca" ) },
  { 569, TEXT( "Syliaca" ) },
  { 570, TEXT( "Grownup Syliaca" ) },
  { 571, TEXT( "Giant Syliaca" ) },
  { 572, TEXT( "Small Zombiger" ) },
  { 573, TEXT( "Zombiger" ) },
  { 575, TEXT( "Captain Zombiger" ) },
  { 576, TEXT( "Giant Zombiger" ) },
  { 577, TEXT( "Small Flybat" ) },
  { 578, TEXT( "Flybat" ) },
  { 579, TEXT( "Captain Flybat" ) },
  { 580, TEXT( "Giant Flybat" ) },
  { 581, TEXT( "Small Bucrow" ) },
  { 582, TEXT( "Bucrow" ) },
  { 583, TEXT( "Captain Bucrow" ) },
  { 584, TEXT( "Giant Bucrow" ) },
  { 585, TEXT( "Small Scorpicon" ) },
  { 586, TEXT( "Scorpicon" ) },
  { 587, TEXT( "Captain Scorpicon" ) },
  { 588, TEXT( "Giant Scorpicon" ) },
  { 589, TEXT( "Small Trangfoma" ) },
  { 590, TEXT( "Trangfoma" ) },
  { 591, TEXT( "Captain Trangfoma" ) },
  { 592, TEXT( "Giant Trangfoma" ) },
  { 593, TEXT( "Guru Catcher" ) },
  { 594, TEXT( "Small Watangka" ) },
  { 595, TEXT( "Watangka" ) },
  { 596, TEXT( "Captain Watangka" ) },
  { 597, TEXT( "Giant Watangka" ) },
  { 598, TEXT( "Small Iren" ) },
  { 599, TEXT( "Iren" ) },
  { 600, TEXT( "Captain Iren" ) },
  { 601, TEXT( "Giant Iren" ) },
  { 602, TEXT( "Small Boo" ) },
  { 603, TEXT( "Boo" ) },
  { 604, TEXT( "Captain Boo" ) },
  { 605, TEXT( "Giant Boo" ) },
  { 606, TEXT( "Small Luia" ) },
  { 607, TEXT( "Luia" ) },
  { 608, TEXT( "Captain Luia" ) },
  { 609, TEXT( "Giant Luia" ) },
  { 610, TEXT( "Small Glaphan" ) },
  { 611, TEXT( "Glaphan" ) },
  { 612, TEXT( "Captain Glaphan" ) },
  { 613, TEXT( "Giant Glaphan" ) },
  { 614, TEXT( "Small Shuhamma" ) },
  { 615, TEXT( "Shuhamma" ) },
  { 616, TEXT( "Captain Shuhamma" ) },
  { 617, TEXT( "Giant Shuhamma" ) },
  { 618, TEXT( "Small Nautrepy" ) },
  { 619, TEXT( "Nautrepy" ) },
  { 620, TEXT( "Captain Nautrepy" ) },
  { 621, TEXT( "Giant Nautrepy" ) },
  { 622, TEXT( "Small Grrr" ) },
  { 623, TEXT( "Grrr" ) },
  { 624, TEXT( "Captain Grrr" ) },
  { 625, TEXT( "Giant Grrr" ) },
  { 626, TEXT( "Small Antiquery" ) },
  { 627, TEXT( "Antiquery" ) },
  { 628, TEXT( "Captain Antiquery" ) },
  { 629, TEXT( "Giant Antiquery" ) },
  { 630, TEXT( "Small Mushpoie" ) },
  { 631, TEXT( "Mushpoie" ) },
  { 632, TEXT( "Captain Mushpoie" ) },
  { 633, TEXT( "Giant Mushpoie" ) },
  { 634, TEXT( "Trainee Chef Muffrin" ) },
  { 635, TEXT( "3rd Chef Muffrin" ) },
  { 636, TEXT( "2nd Chef Muffrin" ) },
  { 637, TEXT( "1st Chef Muffrin" ) },
  { 638, TEXT( "Small Hoppre" ) },
  { 639, TEXT( "Hoppre" ) },
  { 640, TEXT( "Captain Hoppre" ) },
  { 641, TEXT( "Giant Hoppre" ) },
  { 642, TEXT( "Small Gongury" ) },
  { 643, TEXT( "Gongury" ) },
  { 644, TEXT( "Captain Gongury" ) },
  { 645, TEXT( "Giant Gongury" ) },
  { 646, TEXT( "Small Dump" ) },
  { 647, TEXT( "Dump" ) },
  { 648, TEXT( "Captain Dump" ) },
  { 649, TEXT( "Giant Dump" ) },
  { 650, TEXT( "Small Kern" ) },
  { 651, TEXT( "Kern" ) },
  { 652, TEXT( "Captain Kern" ) },
  { 653, TEXT( "Giant Kern" ) },
  { 654, TEXT( "Mutant Fefern" ) },
  { 655, TEXT( "Mutant Fefern King" ) },
  { 656, TEXT( "Mutant Giant 2nd Class Fefern" ) },
  { 657, TEXT( "Mutant Nyangnyang" ) },
  { 658, TEXT( "Mutant Nyangnyang Queen" ) },
  { 659, TEXT( "Mutant Giant Nyangnyang" ) },
  { 660, TEXT( "Mutant Bang" ) },
  { 661, TEXT( "Mutant Boss Bang" ) },
  { 662, TEXT( "Mutant Giant Bang King" ) },
  { 663, TEXT( "Venel Guardian" ) },
  { 664, TEXT( "Worm Veduque" ) },
  { 665, TEXT( "Serus Uriel" ) },
  { 666, TEXT( "Vice Veduque" ) },
  { 667, TEXT( "Guardian NPC" ) },
  { 668, TEXT( "Chaos Guardian NPC" ) },
  { 669, TEXT( "Captain Guard" ) },
  { 670, TEXT( "Criminal Wagsaac" ) },
  { 671, TEXT( "Criminal Mr. Pumpkin" ) },
  { 672, TEXT( "Criminal Gigglebox" ) },
  { 673, TEXT( "Criminal Hobo" ) },
  { 674, TEXT( "Criminal Cardpuppet" ) },
  { 675, TEXT( "Criminal Basque" ) },
  { 676, TEXT( "Criminal Leyena" ) },
  { 677, TEXT( "Criminal Steelknight" ) },
  { 678, TEXT( "Criminal Volt" ) },
  { 679, TEXT( "Criminal Garbagepider" ) },
  { 680, TEXT( "Criminal Greemong" ) },
  { 681, TEXT( "Criminal Hoppre" ) },
  { 682, TEXT( "Criminal Iren" ) },
  { 683, TEXT( "Criminal Watangka" ) },
  { 684, TEXT( "Criminal Luia" ) },
  { 685, TEXT( "Criminal Shuhamma" ) },
  { 686, TEXT( "Criminal Glaphan" ) },
  { 687, TEXT( "Shuraiture" ) },
  { 688, TEXT( "Ren" ) },
  { 689, TEXT( "Sisif" ) },
  { 690, TEXT( "Rubo" ) },
  { 691, TEXT( "Keakoon Worker" ) },
  { 692, TEXT( "Keakoon Guard" ) },
  { 693, TEXT( "Keakoon Worker Leader" ) },
  { 694, TEXT( "Keakoon Guard Leader" ) },
  { 695, TEXT( "Elite Keakoon Worker" ) },
  { 696, TEXT( "Elite Keakoon Guard" ) },
  { 697, TEXT( "Elite Keakoon Worker Leader" ) },
  { 698, TEXT( "Elite Keakoon Guard Leader" ) },
  { 699, TEXT( "Mutant Keakoon Worker" ) },
  { 700, TEXT( "Mutant Keakoon Guard" ) },
  { 701, TEXT( "Mutant Keakoon Worker Leader" ) },
  { 702, TEXT( "Mutant Keakoon Guard Leader" ) },
  { 703, TEXT( "Pink Roach" ) },
  { 704, TEXT( "Blue Roach" ) },
  { 705, TEXT( "Pink Roach Queen" ) },
  { 707, TEXT( "Green Trillipy" ) },
  { 708, TEXT( "Hazardous Green Trillipy" ) },
  { 709, TEXT( "Violet Trillipy" ) },
  { 710, TEXT( "Hazardous Violet Trillipy" ) },
  { 711, TEXT( "Blood Trillipy" ) },
  { 712, TEXT( "Hazardous Blood Trillipy" ) },
  { 713, TEXT( "Chimeradon" ) },
  { 714, TEXT( "Giant Chimeradon" ) },
  { 715, TEXT( "Meteonyker" ) },
  { 716, TEXT( "Pink Roach Larva" ) },
  { 717, TEXT( "Blue Roach Larva" ) },
  { 718, TEXT( "Blue Roach Queen" ) },
  { 719, TEXT( "Criminal Fefern" ) },
  { 720, TEXT( "Baby Lawolf" ) },
  { 721, TEXT( "Baby Aibatt" ) },
  { 722, TEXT( "Baby Leyena" ) },
  { 723, TEXT( "Baby Luia" ) },
  { 724, TEXT( "Kitty" ) },
  { 725, TEXT( "Doggy" ) },
  { 726, TEXT( "Soccerball Angel" ) },
  { 727, TEXT( "Baby Polar Bear" ) },
  { 728, TEXT( "Baby Penguin" ) },
  { 729, TEXT( "Baby Iguana" ) },
  { 730, TEXT( "Baby Cobra" ) },
  { 731, TEXT( "Egg" ) },
  { 732, TEXT( "White Tiger" ) },
  { 733, TEXT( "White Tiger" ) },
  { 734, TEXT( "Babari lion" ) },
  { 735, TEXT( "Babari lion" ) },
  { 736, TEXT( "Rabbit" ) },
  { 737, TEXT( "Rabbit" ) },
  { 738, TEXT( "Dragon" ) },
  { 739, TEXT( "Dragon" ) },
  { 740, TEXT( "Unicorn" ) },
  { 741, TEXT( "Unicorn" ) },
  { 742, TEXT( "Old fox" ) },
  { 743, TEXT( "Old fox" ) },
  { 744, TEXT( "Griffin" ) },
  { 745, TEXT( "Griffin" ) },
  { 746, TEXT( "Cute Frog" ) },
  { 747, TEXT( "Cute Hamburger" ) },
  { 748, TEXT( "Baby Turtle" ) },
  { 750, TEXT( "Chaner" ) },
  { 751, TEXT( "Babari" ) },
  { 752, TEXT( "Seido" ) },
  { 753, TEXT( "Red Meteonyker" ) },
  { 754, TEXT( "Blue Meteonyker" ) },
  { 755, TEXT( "Green Meteonyker" ) },
  { 800, TEXT( "Red Bang Troupe Member" ) },
  { 811, TEXT( "Member of the Violet Magician Troupe" ) },
  { 812, TEXT( "Member of the Violet Magician Troupe" ) },
  { 813, TEXT( "Organigor" ) },
  { 814, TEXT( "Gangard" ) },
  { 815, TEXT( "Hadeseor" ) },
  { 816, TEXT( "Arc Master of the Violet Magician Troupe" ) },
  { 817, TEXT( "Red Bang Troupe Leader" ) },
  { 818, TEXT( "Lord Bang" ) },
  { 819, TEXT( "Hammer Kick" ) },
  { 820, TEXT( "Ant Turtle" ) },
  { 821, TEXT( "Emeraldmantis" ) },
  { 822, TEXT( "Lord Clockworks Alpha" ) },
  { 823, TEXT( "Brigadier General Crumple" ) },
  { 824, TEXT( "Lieutenant General Scythoid" ) },
  { 825, TEXT( "Air Marshall Spiketail" ) },
  { 826, TEXT( "General Glyphaxz" ) },
  { 827, TEXT( "Lord Bang Hanoyan" ) },
  { 828, TEXT( "Soldier Chimeradon" ) },
  { 829, TEXT( "Brigadier Chimeradon" ) },
  { 830, TEXT( "Major Chimeradon" ) },
  { 831, TEXT( "Lieutenant Chimeradon" ) },
  { 832, TEXT( "General Chimeradon" ) },
  { 833, TEXT( "Private Bearnerky" ) },
  { 834, TEXT( "Specialist Bearnerky" ) },
  { 835, TEXT( "Sergeant Bearnerky" ) },
  { 836, TEXT( "Captian Bearnerky" ) },
  { 837, TEXT( "General Bearnerky" ) },
  { 838, TEXT( "Great Chef Muffrin" ) },
  { 839, TEXT( "Queen Popcrank" ) },
  { 840, TEXT( "Mother Yoln" ) },
  { 841, TEXT( "Kidler" ) },
  { 842, TEXT( "Shacalpion" ) },
  { 843, TEXT( "Hoiren" ) },
  { 844, TEXT( "Hunter X" ) },
  { 845, TEXT( "Kynsy" ) },
  { 846, TEXT( "Clocks" ) },
  { 855, TEXT( "Ivillis Crasher" ) },
  { 856, TEXT( "Ivillis Poisoner" ) },
  { 857, TEXT( "Ivillis Wrecker" ) },
  { 858, TEXT( "Ivillis Destroyer" ) },
  { 859, TEXT( "Ivillis Puppet" ) },
  { 860, TEXT( "Ivillis Thief" ) },
  { 861, TEXT( "Ivillis Guardian" ) },
  { 862, TEXT( "Ivillis Quaker" ) },
  { 863, TEXT( "Ivillis Mushellizer" ) },
  { 864, TEXT( "Ivillis Leanes" ) },
  { 865, TEXT( "Ivillis Red Otem" ) },
  { 866, TEXT( "Ivillis Black Otem" ) },
  { 867, TEXT( "Ivillis Boxter" ) },
  { 868, TEXT( "Ivillis Dandysher" ) },
  { 869, TEXT( "Red Angel" ) },
  { 870, TEXT( "Blue Angel" ) },
  { 871, TEXT( "Green Angel" ) },
  { 872, TEXT( "White Angel" ) },
  { 873, TEXT( "Post Box" ) },
  { 874, TEXT( "CyclopsX" ) },
  { 875, TEXT( "Mysterious Robed Boy" ) },
  { 876, TEXT( "Mysterious Robed Girl" ) },
  { 877, TEXT( "Gambler" ) },
  { 878, TEXT( "Hair Designer" ) },
  { 879, TEXT( "Make-Up Artist" ) },
  { 880, TEXT( "Pet Tamer" ) },
  { 881, TEXT( "Dancer" ) },
  { 882, TEXT( "Miss Flyff" ) },
  { 883, TEXT( "Mr. Flyff" ) },
  { 884, TEXT( "[Captain] Behemoth" ) },
  { 885, TEXT( "[Captain] Hellion" ) },
  { 886, TEXT( "[Captain] Jackalope" ) },
  { 887, TEXT( "[Captain] Bahamut" ) },
  { 888, TEXT( "[Captain] Nightmare" ) },
  { 889, TEXT( "[Captain] Reinecke" ) },
  { 890, TEXT( "[Captain] Hippogryph" ) },
  { 891, TEXT( "Priest of Penence" ) },
  { 892, TEXT( "Mysterious Boy" ) },
  { 893, TEXT( "Baby Chicken" ) },
  { 894, TEXT( "Baby Cow" ) },
  { 895, TEXT( "Baby Beagle" ) },
  { 896, TEXT( "Baby Blue Dragon" ) },
  { 897, TEXT( "Baby Hamster" ) },
  { 898, TEXT( "Baby Horse" ) },
  { 899, TEXT( "Baby Monkey" ) },
  { 900, TEXT( "Baby Pig" ) },
  { 901, TEXT( "Baby Rabbit" ) },
  { 902, TEXT( "Cute Lamb" ) },
  { 903, TEXT( "Baby Tiger" ) },
  { 904, TEXT( "Yetti" ) },
  { 905, TEXT( "Mutant Yetti" ) },
  { 906, TEXT( "Augu" ) },
  { 907, TEXT( "Mutant Augu" ) },
  { 908, TEXT( "Ghost of the Forgotten Prince" ) },
  { 909, TEXT( "Ghost of the Forgotten King" ) },
  { 910, TEXT( "Waforu" ) },
  { 911, TEXT( "Mammoth " ) },
  { 912, TEXT( "Cannibal Mammoth" ) },
  { 913, TEXT( "Collins" ) },
  { 914, TEXT( "Small Maid Cardpuppet" ) },
  { 915, TEXT( "Small Mia" ) },
  { 916, TEXT( "Baby Black Dragon" ) },
  { 917, TEXT( "GM Lysis" ) },
  { 918, TEXT( "GM Demian" ) },
  { 919, TEXT( "GM Redsun" ) },
  { 920, TEXT( "GM Cromiell" ) },
  { 921, TEXT( "Target" ) },
  { 922, TEXT( "Small Harpy" ) },
  { 923, TEXT( "Harpy" ) },
  { 924, TEXT( "Captain Harpy" ) },
  { 925, TEXT( "Giant Harpy" ) },
  { 926, TEXT( "Small Gullah" ) },
  { 927, TEXT( "Gullah" ) },
  { 928, TEXT( "Captain Gullah" ) },
  { 929, TEXT( "Giant Gullah" ) },
  { 930, TEXT( "Small Abraxas" ) },
  { 931, TEXT( "Abraxas" ) },
  { 932, TEXT( "Captain Abraxas" ) },
  { 933, TEXT( "Giant Abraxas" ) },
  { 934, TEXT( "Small Hague" ) },
  { 935, TEXT( "Hague" ) },
  { 936, TEXT( "Captain Hague" ) },
  { 937, TEXT( "Giant Hague" ) },
  { 938, TEXT( "Small Tengu" ) },
  { 939, TEXT( "Tengu" ) },
  { 940, TEXT( "Captain Tengu" ) },
  { 941, TEXT( "Giant Tengu" ) },
  { 942, TEXT( "Small Asterius" ) },
  { 943, TEXT( "Asterius" ) },
  { 944, TEXT( "Captain Asterius" ) },
  { 945, TEXT( "Giant Asterius" ) },
  { 946, TEXT( "Small Dantalian" ) },
  { 947, TEXT( "Dantalian" ) },
  { 948, TEXT( "Captain Dantalian" ) },
  { 949, TEXT( "Giant Dantalian" ) },
  { 950, TEXT( "Small Gannessa" ) },
  { 951, TEXT( "Gannessa" ) },
  { 952, TEXT( "Captain Gannessa" ) },
  { 953, TEXT( "Giant Gannessa" ) },
  { 954, TEXT( "Small Asuras" ) },
  { 955, TEXT( "Asuras" ) },
  { 956, TEXT( "Captain Asuras" ) },
  { 957, TEXT( "Giant Asuras" ) },
  { 958, TEXT( "Small Catsy" ) },
  { 959, TEXT( "Catsy" ) },
  { 960, TEXT( "Captain Catsy" ) },
  { 961, TEXT( "Giant Catsy" ) },
  { 962, TEXT( "Small Nyx" ) },
  { 963, TEXT( "Nyx" ) },
  { 964, TEXT( "Captain Nyx" ) },
  { 965, TEXT( "Asmodan" ) },
  { 966, TEXT( "Rangda" ) },
  { 967, TEXT( "Horrible Rangda" ) },
  { 968, TEXT( "Uncanny Rangda" ) },
  { 969, TEXT( "Dreadful Rangda" ) },
  { 970, TEXT( "Baby Yetti" ) },
  { 971, TEXT( "Great Catsy" ) },
  { 972, TEXT( "Great Harpy" ) },
  { 973, TEXT( "Great Gullah" ) },
  { 974, TEXT( "Great Abraxas" ) },
  { 975, TEXT( "Great Hague" ) },
  { 976, TEXT( "Great Tengu" ) },
  { 977, TEXT( "Great Asterius" ) },
  { 978, TEXT( "Great Dantalian" ) },
  { 979, TEXT( "Great Gannessa" ) },
  { 980, TEXT( "Great Asuras" ) },
  { 981, TEXT( "Knight 2" ) },
  { 982, TEXT( "Knight 1" ) },
  { 983, TEXT( "Lord Manager" ) },
  { 984, TEXT( "Ice Lady" ) },
  { 985, TEXT( "[Spirit of Rainbow] Red" ) },
  { 986, TEXT( "[Spirit of Rainbow] Orange" ) },
  { 987, TEXT( "[Spirit of Rainbow] Yellow" ) },
  { 988, TEXT( "[Spirit of Rainbow] Green" ) },
  { 989, TEXT( "[Spirit of Rainbow] Blue" ) },
  { 990, TEXT( "[Spirit of Rainbow] Indigo" ) },
  { 991, TEXT( "[Spirit of Rainbow] Violet" ) },
  { 992, TEXT( "Rainbow Race Officer" ) },
  { 993, TEXT( "Small Kingster" ) },
  { 994, TEXT( "Kingster" ) },
  { 995, TEXT( "Captain Kingster" ) },
  { 996, TEXT( "Small Kraken" ) },
  { 997, TEXT( "Kraken" ) },
  { 998, TEXT( "Captain Kraken" ) },
  { 999, TEXT( "Small Creper" ) },
  { 1000, TEXT( "Creper" ) },
  { 1001, TEXT( "Captain Creper" ) },
  { 1002, TEXT( "Small Naga" ) },
  { 1003, TEXT( "Naga" ) },
  { 1004, TEXT( "Captain Naga" ) },
  { 1005, TEXT( "Small Atrox" ) },
  { 1006, TEXT( "Atrox" ) },
  { 1007, TEXT( "Captain Atrox" ) },
  { 1008, TEXT( "Small Okean" ) },
  { 1009, TEXT( "Okean" ) },
  { 1010, TEXT( "Captain Okean" ) },
  { 1011, TEXT( "Small Tigar" ) },
  { 1012, TEXT( "Tigar" ) },
  { 1013, TEXT( "Captain Tigar" ) },
  { 1014, TEXT( "Small Dorian" ) },
  { 1015, TEXT( "Dorian" ) },
  { 1016, TEXT( "Captain Dorian" ) },
  { 1017, TEXT( "Small Meral" ) },
  { 1018, TEXT( "Meral" ) },
  { 1019, TEXT( "Captain Meral" ) },
  { 1021, TEXT( "Guard" ) },
  { 1022, TEXT( "[Guildwar Manager] Frankie" ) },
  { 1023, TEXT( "[Guildwar Manager] Donaris" ) },
  { 1024, TEXT( "[1vs1Guildwar Manager] Amos" ) },
  { 1025, TEXT( "[1vs1Guildwar Manager] Annie" ) },
  { 1026, TEXT( "[Arena Manager] Ray" ) },
  { 1027, TEXT( "Mayor" ) },
  { 1028, TEXT( "Charlie" ) },
  { 1029, TEXT( "Baby Snowman" ) },
  { 1030, TEXT( "Daddy Snowman" ) },
  { 1031, TEXT( "English Bulldog" ) },
  { 1032, TEXT( "Naughty Ghost" ) },
  { 1033, TEXT( "Baby Fu Lion" ) },
  { 1034, TEXT( "Smart Owl" ) },
  { 1035, TEXT( "Patches" ) },
  { 1036, TEXT( "Baby Robot" ) },
  { 1037, TEXT( "Baby Bang" ) },
  { 1038, TEXT( "Baby Panda" ) },
  { 1039, TEXT( "Red Armored Tigar" ) },
  { 1040, TEXT( "Blue Armored Tigar" ) },
  { 1041, TEXT( "Yellow Armored Tigar" ) },
  { 1042, TEXT( "Small Club-tailed Reptilion " ) },
  { 1043, TEXT( "Club-tailed Reptilion " ) },
  { 1044, TEXT( "Captain Club-tailed Reptilion " ) },
  { 1045, TEXT( "Colonel Club-tailed Reptilion " ) },
  { 1046, TEXT( "Lazy Araknoid" ) },
  { 1047, TEXT( "Araknoid" ) },
  { 1048, TEXT( "Creepy Araknoid" ) },
  { 1049, TEXT( "Giant Araknoid" ) },
  { 1050, TEXT( "Small Battle Toadrin" ) },
  { 1051, TEXT( "Battle Toadrin" ) },
  { 1052, TEXT( "Creepy Battle Toadrin" ) },
  { 1053, TEXT( "Giant Battle Toadrin" ) },
  { 1054, TEXT( "Brown Bolo" ) },
  { 1055, TEXT( "Spotted Bolo" ) },
  { 1056, TEXT( "Superior Spotted Bolo" ) },
  { 1057, TEXT( "Great White Bolo" ) },
  { 1058, TEXT( "Troglodon Clansman" ) },
  { 1059, TEXT( "Troglodon Warrior" ) },
  { 1060, TEXT( "Troglodon Captain" ) },
  { 1061, TEXT( "Troglodon Warlord" ) },
  { 1062, TEXT( "Blunt Axe-Jaw Ant" ) },
  { 1063, TEXT( "Axe-Jaw Ant" ) },
  { 1064, TEXT( "Sharp Axe-Jaw Ant" ) },
  { 1065, TEXT( "Razor Axe-Jaw Ant" ) },
  { 1066, TEXT( "Small Scorpede" ) },
  { 1067, TEXT( "Scorpede" ) },
  { 1068, TEXT( "Creepy Scorpede" ) },
  { 1069, TEXT( "Giant Scorpede" ) },
  { 1070, TEXT( "Small Maul Rat" ) },
  { 1071, TEXT( "Maul Rat" ) },
  { 1072, TEXT( "Creepy Maul Rat" ) },
  { 1073, TEXT( "Giant Maul Rat" ) },
  { 1074, TEXT( "Lykanos the Malevolent" ) },
  { 1075, TEXT( "Drakul the Diabolic" ) },
  { 1076, TEXT( "Cursed Blunt Axe-Jaw Ant" ) },
  { 1077, TEXT( "Cursed Axe-Jaw Ant" ) },
  { 1078, TEXT( "Cursed Sharp Axe-Jaw Ant" ) },
  { 1079, TEXT( "Cursed Razor Axe-Jaw Ant" ) },
  { 1080, TEXT( "Cursed Small Scorpede" ) },
  { 1081, TEXT( "Cursed Scorpede" ) },
  { 1082, TEXT( "Cursed Creepy Scorpede" ) },
  { 1083, TEXT( "Cursed Giant Scorpede" ) },
  { 1084, TEXT( "Cursed Small Maul Rat" ) },
  { 1085, TEXT( "Cursed Maul Rat" ) },
  { 1086, TEXT( "Cursed Creepy Maul Rat" ) },
  { 1087, TEXT( "Cursed Giant Maul Rat" ) },
  { 1088, TEXT( "Dread Lykanos the Malevolent" ) },
  { 1089, TEXT( "Dread Drakul the Diabolic" ) },
  { 1090, TEXT( "[Adventure Assitant] Jano" ) },
  { 1091, TEXT( "[Adventure Captain] Bespoo" ) },
  { 1092, TEXT( "Luka" ) },
  { 1093, TEXT( "Lasa" ) },
  { 1094, TEXT( "Ryan" ) },
  { 1095, TEXT( "Tommy Toucan" ) },
  { 1096, TEXT( "Ete" ) },
  { 1097, TEXT( "Tong" ) },
  { 1098, TEXT( "Johnny Four-Wheels" ) },
  { 1099, TEXT( "Miss Vietnam" ) },
  { 1100, TEXT( "Mocomochi" ) },
  { 1101, TEXT( "[Teleporter] Master" ) },
  { 1102, TEXT( "Tiny Tanuki" ) },
  { 1104, TEXT( "Sellia" ) },
  { 1105, TEXT( "Inisis" ) },
  { 1106, TEXT( "Hellhound" ) },
  { 1107, TEXT( "Risen Gladiator" ) },
  { 1108, TEXT( "Risen Pikeman" ) },
  { 1109, TEXT( "Risen Mage" ) },
  { 1110, TEXT( "Risen Assassin" ) },
  { 1111, TEXT( "Risen Warrior" ) },
  { 1112, TEXT( "[General] Razgul" ) },
  { 1113, TEXT( "Blighted Gryphon " ) },
  { 1114, TEXT( "Scythe Protector" ) },
  { 1115, TEXT( "Vile Flayer" ) },
  { 1116, TEXT( "Fallen Necromancer" ) },
  { 1117, TEXT( "Forsaken Banshee" ) },
  { 1118, TEXT( "[Deathbringer] Kheldor" ) },
  { 1119, TEXT( "[Teleporter] Apprentice" ) },
  { 1120, TEXT( "Mini Santa" ) },
  { 1121, TEXT( "Mini Snowman" ) },
  { 1122, TEXT( "Bellosia" ) },
  { 1123, TEXT( "Loraine" ) },
  { 1124, TEXT( "Little Lykan" ) },
  { 1125, TEXT( "Idol of the Blighted Gryphon" ) },
  { 1126, TEXT( "Idol of the Scythe Protector" ) },
  { 1127, TEXT( "Idol of the Vile Flayer" ) },
  { 1128, TEXT( "Idol of the Fallen Necromancer" ) },
  { 1129, TEXT( "Idol of the Forsaken Banshee" ) },
  { 1130, TEXT( "Idol of Kheldor" ) },
  { 1131, TEXT( "Prismatic Cobra" ) },
  { 1132, TEXT( "Twinstrike Cobra" ) },
  { 1133, TEXT( "Tanuki Enforcer" ) },
  { 1134, TEXT( "Tanuki Protector" ) },
  { 1135, TEXT( "Wildwood Stalker" ) },
  { 1136, TEXT( "Wildwood Shaman" ) },
  { 1137, TEXT( "Beast King Khan" ) },
  { 1138, TEXT( "Mythic Prismatic Cobra" ) },
  { 1139, TEXT( "Mythic Twinstrike Cobra" ) },
  { 1140, TEXT( "Elite Tanuki Enforcer" ) },
  { 1141, TEXT( "Elite Tanuki Protector" ) },
  { 1142, TEXT( "Mythic Wildwood Stalker" ) },
  { 1143, TEXT( "Shadowy Wildwood Shaman" ) },
  { 1144, TEXT( "Beast Overlord Khan" ) },
  { 1145, TEXT( "Water Totem pole" ) },
  { 1146, TEXT( "Wind Totem pole" ) },
  { 1147, TEXT( "Earth Totem pole" ) },
  { 1148, TEXT( "Fire Totem pole" ) },
  { 1149, TEXT( "Joey" ) },
  { 1150, TEXT( "Spike" ) },
  { 1151, TEXT( "Lumia" ) },
  { 1152, TEXT( "Inspector Koko" ) },
  { 1153, TEXT( "Leprechaun" ) },
  { 1154, TEXT( "Cheshire Cat" ) },
  { 1155, TEXT( "The White Rabbit " ) },
  { 1156, TEXT( "Tiger Cub" ) },
  { 1157, TEXT( "Sakai" ) },
  { 1158, TEXT( "Vile Thorn" ) },
  { 1159, TEXT( "Grumble Mauler" ) },
  { 1160, TEXT( "Blackweb Shade" ) },
  { 1161, TEXT( "Immovable Crag" ) },
  { 1162, TEXT( "Dire Razor" ) },
  { 1163, TEXT( "Guan Yu Heavyblade" ) },
  { 1164, TEXT( "Laveau" ) },
  { 1165, TEXT( "Bone Golem" ) },
  { 1166, TEXT( "Mocomochi" ) },
  { 1167, TEXT( "Soul Dancer" ) },
  { 1168, TEXT( "Clockworks Butler" ) },
  { 1169, TEXT( "[Twisted Soul] Orcus" ) },
  { 1170, TEXT( "[Tormented Soul] Nergal" ) },
  { 1171, TEXT( "[Demonic Soul] Hel" ) },
  { 1172, TEXT( "[Perverted Soul] Morrigan" ) },
  { 1173, TEXT( "[Anguished Soul] Mara" ) },
  { 1174, TEXT( "[Violent Soul] Ghed " ) },
  { 1175, TEXT( "[God of Death] Ankou" ) },
  { 1210, TEXT( "Monument of Death" ) },
  { 1176, TEXT( "Hundur Sharpfoot" ) },
  { 1177, TEXT( "Samoset" ) },
  { 1178, TEXT( "Kyouchish" ) },
  { 1179, TEXT( "Kanonicus" ) },
  { 1180, TEXT( "Mage Redcloud" ) },
  { 1181, TEXT( "Taiaha" ) },
  { 1182, TEXT( "Chief Keokuk" ) },
  { 1183, TEXT( "Dumb Bull Matador" ) },
  { 1184, TEXT( "Rampaging Dumb Bull" ) },
  { 1185, TEXT( "[Public Office] Ruidan" ) },
  { 1186, TEXT( "[Village Chief] Huntrang" ) },
  { 1187, TEXT( "[Quest Office] Miorang" ) },
  { 1188, TEXT( "[Magic] Mawrang" ) },
  { 1189, TEXT( "[Weapon] Kanrang" ) },
  { 1190, TEXT( "[Arena Manager] Rarbadan" ) },
  { 1191, TEXT( "Rergantus" ) },
  { 1192, TEXT( "Miriun" ) },
  { 1193, TEXT( "[Armor] Karang" ) },
  { 1194, TEXT( "[Shield] Kurang" ) },
  { 1195, TEXT( "[Station] Heirang" ) },
  { 1196, TEXT( "[Food] Arang" ) },
  { 1197, TEXT( "[General] Maurang" ) },
  { 1198, TEXT( "[Force Master] Bato" ) },
  { 1199, TEXT( "[Crackshooter Master] Meiarun" ) },
  { 1200, TEXT( "[Arcanist Master] Howrang" ) },
  { 1201, TEXT( "[Upgrade Specialist] Rutam" ) },
  { 1202, TEXT( "[Jewel Manager] Raia" ) },
  { 1203, TEXT( "[Housing Specialist] Harlie" ) },
  { 1204, TEXT( "[Guild House Manager] Rodella" ) },
  { 1205, TEXT( "[Card Master] Epie" ) },
  { 1206, TEXT( "[Makeup Artist] Huri" ) },
  { 1207, TEXT( "[Hair Designer] Gelga" ) },
  { 1208, TEXT( "[Red Chip Merchant] Ruoboru" ) },
  { 1212, TEXT( "Cheirang" ) },
  { 1213, TEXT( "Emblum" ) },
  { 1214, TEXT( "Karl" ) },
  { 1215, TEXT( "Branka" ) },
  { 1216, TEXT( "Isillis" ) },
  { 1217, TEXT( "Romain" ) },
  { 1218, TEXT( "Muran Defender" ) },
  { 1219, TEXT( "Muran Guardian" ) },
  { 1209, TEXT( "Needle Tail Dragon" ) },
  { 1211, TEXT( "[Legendary Golden Weapon Vendor] Romina" ) },
  { 1220, TEXT( "Piyo" ) },
  { 1221, TEXT( "Udi" ) },
  { 1222, TEXT( "[Event] Baby Sheep" ) },
  { 1274, TEXT( "No info" ) },
  { 1275, TEXT( "Myorang" ) },
  { 1276, TEXT( "Event Mia" ) },
  { 1277, TEXT( "Event Cardpuppet" ) },
  { 1278, TEXT( "Event Rangda" ) },
  { 1279, TEXT( "Pinata" ) },
  { 1280, TEXT( "[Rock Scissor Paper NPC]Demian" ) },
  { 1281, TEXT( "Chameleon " ) },
  { 1282, TEXT( "Light" ) },
  { 1283, TEXT( "Dark" ) },
  { 1284, TEXT( "[Guild House Manager] Loa" ) },
  { 1285, TEXT( "[Guild House Manager] Troy" ) },
  { 1303, TEXT( "Romero" ) },
  { 1304, TEXT( "Kirkman" ) },
  { 1322, TEXT( "[Event NPC]Big Buff Pang" ) },
  { 1323, TEXT( "R. DeFeo" ) },
  { 1324, TEXT( "Bloody Mary" ) },
  { 1325, TEXT( "Braaaaains" ) },
  { 1327, TEXT( "Brooks" ) },
  { 1328, TEXT( "ATMA" ) },
  { 1329, TEXT( "[Exchange NPC] Chance" ) },
  { 1330, TEXT( "Male Zombie" ) },
  { 1331, TEXT( "Female Zombie " ) },
  { 1335, TEXT( "Gobbler" ) },
  { 1336, TEXT( "Big Red" ) },
  { 1337, TEXT( "[Expedition Leader] Rukas" ) },
  { 1338, TEXT( "Pinanoco" ) },
  { 1339, TEXT( "Raundas" ) },
  { 1340, TEXT( "Basilisk of the Great Maw" ) },
  { 1341, TEXT( "Wild Sunbaked Rhino" ) },
  { 1342, TEXT( "Sunbaked Rhino Warrior" ) },
  { 1343, TEXT( "Desert Spirit of the Commander" ) },
  { 1344, TEXT( "Desert Spirit of the Fallen Mage" ) },
  { 1345, TEXT( "Desert Spirit of the Sage" ) },
  { 1346, TEXT( "Warrior Sand Skitter " ) },
  { 1347, TEXT( "[Kalgas Guardian] Astaroth " ) },
  { 1348, TEXT( "Baby Kargo" ) },
  { 1349, TEXT( "[Kalgas Guardian] Malphas" ) },
  { 1350, TEXT( "Kalgas Kargo Ripper" ) },
  { 1351, TEXT( "Kalgas Cave Creeper " ) },
  { 1352, TEXT( "Kalgas Hound" ) },
  { 1353, TEXT( "Kalgas Wretched Soul" ) },
  { 1354, TEXT( "[God of Wrath] Kalgas" ) },
  { 1355, TEXT( "Desert Spirit of the Fallen Soldier" ) },
  { 1356, TEXT( "Wild Sand Skitter" ) },
  { 1357, TEXT( "Idelruna" ) },
  { 1358, TEXT( "Gigaka" ) },
  { 1359, TEXT( "[Mirrored Soul] Giant Bang" ) },
  { 1360, TEXT( "[Mirrored Soul] Giant Wagsaac" ) },
  { 1361, TEXT( "[Mirrored Soul] Giant Red Mantis" ) },
  { 1362, TEXT( "[Mirrored Soul] Giant Jack the Hammer" ) },
  { 1363, TEXT( "[Mirrored Soul] Giant Bucrow" ) },
  { 1364, TEXT( "[Mirrored Soul] Giant Hobo" ) },
  { 1365, TEXT( "[Mirrored Soul] Giant Rockepeller " ) },
  { 1366, TEXT( "[Mirrored Soul] Giant Steam Walker" ) },
  { 1367, TEXT( "[Mirrored Soul] Giant Grrr" ) },
  { 1368, TEXT( "[Mirrored Soul] Giant Greemong" ) },
  { 1369, TEXT( "[Mirrored Soul] Giant Luia" ) },
  { 1370, TEXT( "[Mirrored Soul] Giant Gongury" ) },
  { 1371, TEXT( "[Mirrored Soul] Giant Kern" ) },
  { 1372, TEXT( "[Mirrored Soul] Meteonyker" ) },
  { 1373, TEXT( "[Mirrored Soul] CyclopsX" ) },
  { 1374, TEXT( "[Mirrored Soul] Mara" ) },
  { 1375, TEXT( "[Mirrored Soul] Tanuki " ) },
  { 1376, TEXT( "[Mirrored Soul] Red Meteonyker" ) },
  { 1377, TEXT( "[Mirrored Soul] Clockworks" ) },
  { 1378, TEXT( "[Mirrored Soul] Khan" ) },
  { 1379, TEXT( "[Mirrored Soul] Asmodan" ) },
  { 1380, TEXT( "[Mirrored Soul] Lykanose" ) },
  { 1381, TEXT( "[Mirrored Soul] Drakul" ) },
  { 1382, TEXT( "[Mirrored Soul] Razgul" ) },
  { 1383, TEXT( "[Mirrored Soul] Guan Yu Heavyblade" ) },
  { 1384, TEXT( "[Mirrored Soul] Keokuk" ) },
  { 1385, TEXT( "[Mirrored Soul] Ankou" ) },
  { 1386, TEXT( "[Mirrored Soul] Basilisk of the Great Maw" ) },
  { 1387, TEXT( "[Mirrored Soul] Kalgas" ) },
  { 1388, TEXT( "[Mirrored Soul] Kheldor" ) },
  { 1389, TEXT( "Jimbo the Snowman" ) },
  { 1391, TEXT( "[Inferior Soul] Giant Bang" ) },
  { 1392, TEXT( "[Inferior Soul] Giant Wagsaac" ) },
  { 1393, TEXT( "[Inferior Soul] Giant Red Mantis" ) },
  { 1394, TEXT( "[Inferior Soul] Giant Jack the Hammer" ) },
  { 1395, TEXT( "[Inferior Soul] Giant Bucrow" ) },
  { 1396, TEXT( "[Inferior Soul] Giant Hobo" ) },
  { 1397, TEXT( "[Inferior Soul] Giant Rockepeller" ) },
  { 1398, TEXT( "[Inferior Soul] Giant Steam Walker" ) },
  { 1399, TEXT( "[Inferior Soul] Giant Grrr" ) },
  { 1400, TEXT( "[Inferior Soul] Giant Greemong" ) },
  { 1401, TEXT( "[Inferior Soul] Giant Luia" ) },
  { 1402, TEXT( "[Inferior Soul] Giant Gongury" ) },
  { 1403, TEXT( "[Inferior Soul] Giant Kern" ) },
  { 1404, TEXT( "[False Soul] Meteonyker" ) },
  { 1405, TEXT( "[False Soul] CyclopsX" ) },
  { 1406, TEXT( "[False Soul] Mara" ) },
  { 1407, TEXT( "[False Soul] Tanuki" ) },
  { 1408, TEXT( "[False Soul] Red Meteonkyer" ) },
  { 1409, TEXT( "[False Soul] Clockworks" ) },
  { 1410, TEXT( "[False Soul] Khan" ) },
  { 1411, TEXT( "[False Soul] Asmodan" ) },
  { 1412, TEXT( "[False Soul] Lykanose" ) },
  { 1413, TEXT( "[False Soul] Drakul" ) },
  { 1414, TEXT( "[False Soul] Razgul" ) },
  { 1415, TEXT( "[False Soul] Guan Yu Heavyblade" ) },
  { 1416, TEXT( "[False Soul] Keokuk" ) },
  { 1417, TEXT( "[False Soul] Ankou" ) },
  { 1418, TEXT( "[False Soul] Basilisk of the Great Maw" ) },
  { 1419, TEXT( "[False Soul] Kalgas" ) },
  { 1420, TEXT( "[False Soul] Kheldor" ) },
  { 1421, TEXT( "Kalgas Statue" ) },
  { 1422, TEXT( "Akin Statue" ) },
  { 1423, TEXT( "Balt Statue" ) },
  { 1424, TEXT( "Buur" ) },
  { 1425, TEXT( "Saphyryan" ) },
  { 1426, TEXT( "Gray Earl" ) },
  { 1427, TEXT( "Jack Frost" ) },
  { 1428, TEXT( "Mr. Grinchie" ) },
  { 1429, TEXT( "Rudolph" ) },
  { 1430, TEXT( "Event Rocketfeller" ) },
  { 1431, TEXT( "Event Mosbie" ) },
  { 1432, TEXT( "Rex gPotato" ) },
  { 1436, TEXT( "Maximus Rex gPotato " ) },
  { 1433, TEXT( "Event Baby Cargo" ) },
  { 1434, TEXT( "[Development Supervisor] Kim" ) },
  { 1435, TEXT( "[Development Supervisor] Producer Kim" ) },
  { 1437, TEXT( "Kuma" ) },
  { 1438, TEXT( "[Upgrade Spirit] Coby" ) },
  { 1439, TEXT( "[Upgrade Spirit] Pierce" ) },
  { 1440, TEXT( "Love Thief" ) },
  { 1441, TEXT( "Usagi" ) },
  { 1442, TEXT( "Cooperative Donation NPC" ) },
  { 1443, TEXT( "X900 EvilTron" ) },
  { 1444, TEXT( "Mister Grizz" ) },
  { 1445, TEXT( "Silverwing Duchess " ) },
  { 1446, TEXT( "Mouzer" ) },
  { 1447, TEXT( "Winged Mouzer" ) },
  { 1448, TEXT( "Regal Hawk" ) },
  { 1449, TEXT( "Regal Raptor" ) },
  { 1450, TEXT( "Crimson Garuda" ) },
  { 1451, TEXT( "Veiled Garuda " ) },
  { 1452, TEXT( "Phalanx" ) },
  { 1453, TEXT( "Blossom" ) },
  { 1454, TEXT( "Mushbro" ) },
  { 1455, TEXT( "Oldroot" ) },
  { 1456, TEXT( "Minimusha" ) },
  { 1457, TEXT( "Lady Bloom" ) },
  { 1458, TEXT( "Nightmyst" ) },
  { 1459, TEXT( "[Queen of Thorns] Entaness" ) },
  { 1460, TEXT( "Razor Maw" ) },
  { 1461, TEXT( "Kappa" ) },
  { 1462, TEXT( "Nereid" ) },
  { 1463, TEXT( "Terrorpin" ) },
  { 1464, TEXT( "Bunyip" ) },
  { 1465, TEXT( "Selkie" ) },
  { 1466, TEXT( "Siren" ) },
  { 1467, TEXT( "[Terror] King Chiton" ) },
  { 1468, TEXT( "Ace Silverwing Dutchess" ) },
  { 1469, TEXT( "Ace Mouzer " ) },
  { 1470, TEXT( "Ace Winged Mouzer" ) },
  { 1471, TEXT( "Ace Regal Hawk" ) },
  { 1472, TEXT( "Ace Regal Raptor" ) },
  { 1473, TEXT( "Ace Crimson Garuda" ) },
  { 1474, TEXT( "Ace Veiled Garuda" ) },
  { 1475, TEXT( "Ace Phalanx" ) },
  { 1476, TEXT( "Vile Blossom" ) },
  { 1477, TEXT( "Vile Mushbro" ) },
  { 1478, TEXT( "Vile Oldroot" ) },
  { 1479, TEXT( "Vile Minimusha" ) },
  { 1480, TEXT( "Vile Lady Bloom" ) },
  { 1481, TEXT( "Vile Nightmyst" ) },
  { 1482, TEXT( "[Vile Queen of Thorns] Entaness" ) },
  { 1483, TEXT( "Abyssal Razor Maw" ) },
  { 1484, TEXT( "Abyssal Kappa" ) },
  { 1485, TEXT( "Abyssal Nereid" ) },
  { 1486, TEXT( "Abyssal Terrorpin" ) },
  { 1487, TEXT( "Abyssal Bunyip" ) },
  { 1488, TEXT( "Abyssal Selkie" ) },
  { 1489, TEXT( "Abyssal Siren" ) },
  { 1490, TEXT( "[Abyssal Terror] King Chiton" ) },
  { 4010, TEXT( "Mischievous Present Box" ) },

};

#define MI_DEFAULT 10
#define MI_MALE 11
#define MI_FEMALE 12

#define MI_AIBATT1 20
#define MI_AIBATT2 21
#define MI_AIBATT3 22
#define MI_AIBATT4 23
#define MI_BURUDENG1 24
#define MI_BURUDENG2 25
#define MI_BURUDENG3 26
#define MI_BURUDENG4 27
#define MI_PUKEPUKE1 28
#define MI_PUKEPUKE2 29
#define MI_PUKEPUKE3 30
#define MI_PUKEPUKE4 31
#define MI_DORIDOMA1 32
#define MI_DORIDOMA2 33
#define MI_DORIDOMA3 34
#define MI_DORIDOMA4 35
#define MI_LAWOLF1 36
#define MI_LAWOLF2 37
#define MI_LAWOLF3 38
#define MI_LAWOLF4 39
#define MI_NYANGNYANG1 40
#define MI_NYANGNYANG2 41
#define MI_NYANGNYANG3 42
#define MI_NYANGNYANG4 43
#define MI_BANG1 44
#define MI_BANG2 45
#define MI_BANG3 46
#define MI_BANG4 47
#define MI_WAGSAAC1 48
#define MI_WAGSAAC2 49
#define MI_WAGSAAC3 50
#define MI_WAGSAAC4 51
#define MI_WHEELEM1 52
#define MI_WHEELEM2 53
#define MI_WHEELEM3 54
#define MI_WHEELEM4 55
#define MI_TOMBSTONEBEARER1 56
#define MI_TOMBSTONEBEARER2 57
#define MI_TOMBSTONEBEARER3 58
#define MI_TOMBSTONEBEARER4 59
#define MI_FEFERN1 60
#define MI_FEFERN2 61
#define MI_FEFERN3 62
#define MI_FEFERN4 63
#define MI_REDMANTIS1 64
#define MI_REDMANTIS2 65
#define MI_REDMANTIS3 66
#define MI_REDMANTIS4 67
#define MI_MRPUMPKIN1 68
#define MI_MRPUMPKIN2 69
#define MI_MRPUMPKIN3 70
#define MI_MRPUMPKIN4 71
#define MI_BASQUE1 72
#define MI_BASQUE2 73
#define MI_BASQUE3 74
#define MI_BASQUE4 75
#define MI_SCAMP1 76
#define MI_SCAMP2 77
#define MI_SCAMP3 78
#define MI_SCAMP4 79
#define MI_PRANKSTER1 80
#define MI_PRANKSTER2 81
#define MI_PRANKSTER3 82
#define MI_PRANKSTER4 83
#define MI_CARDPUPPET1 84
#define MI_CARDPUPPET2 85
#define MI_CARDPUPPET3 86
#define MI_CARDPUPPET4 87
#define MI_DEMIAN1 88
#define MI_DEMIAN2 89
#define MI_DEMIAN3 90
#define MI_DEMIAN4 91
#define MI_ROCKMUSCLE1 92
#define MI_ROCKMUSCLE2 93
#define MI_ROCKMUSCLE3 94
#define MI_ROCKMUSCLE4 95
#define MI_MUSHPANG1 96
#define MI_MUSHPANG2 97
#define MI_MUSHPANG3 98
#define MI_MUSHPANG4 99
#define MI_PUKEPUKE5 100
#define MI_BANG5 101
#define MI_ROCKMUSCLE5 102
#define MI_TOTEMIA1 103
#define MI_TOTEMIA2 104
#define MI_TOTEMIA3 105
#define MI_TOTEMIA4 106
#define MI_STEAMWALKER1 107
#define MI_STEAMWALKER2 108
#define MI_STEAMWALKER3 109
#define MI_STEAMWALKER4 110
#define MI_AIBATT5 111

#define MI_VOLT1 112
#define MI_VOLT2 113
#define MI_VOLT3 114
#define MI_VOLT4 115
#define MI_CIRCUSBEAR1 116
#define MI_CIRCUSBEAR2 117
#define MI_CIRCUSBEAR3 118
#define MI_CIRCUSBEAR4 119
#define MI_MIA1 120
#define MI_MIA2 121
#define MI_MIA3 122
#define MI_MIA4 123

#define MI_PIRE1 130
#define MI_PIRE2 131
#define MI_PIRE3 132
#define MI_PIRE4 133
#define MI_FROZIEST1 134
#define MI_FROZIEST2 135
#define MI_FROZIEST3 136
#define MI_FROZIEST4 137

#define MI_BURNBIRD1 160
#define MI_BURNBIRD2 161
#define MI_BURNBIRD3 162
#define MI_BURNBIRD4 163
#define MI_CLOCKWORK1 164
#define MI_BIGMUSCLE 165
#define MI_KRRR 166
#define MI_MUSHMOOT 167
#define MI_SMALL_MUSHPOIE 168

#define MI_BUKETFOOT1 190
#define MI_BUKETFOOT2 191
#define MI_BUKETFOOT3 192
#define MI_BUKETFOOT4 193

#define MI_DEMIAN5 196
#define MI_KEAKOON5 197
#define MI_MUFFRIN5 198
#define MI_MRPUMPKIN5 199
#define MI_INFO_PENG 200
#define MI_DWARPET 201
#define MI_DWARPETMAS 202
#define MI_CROWNIBLIS 203
#define MI_CROWNSHADE 204
#define MI_CROWNBUBBLE 205
#define MI_ZOMBIGER5 206

#define MI_MAFL_LOSHA 210
#define MI_MAFL_BOBOKU 211
#define MI_MAFL_JURIA 212
#define MI_MAFL_LUI 213
#define MI_MAFL_MARCHE 214

#define MI_MASA_BULROX 215
#define MI_MASA_TINA 216
#define MI_MASA_KARIN 217
#define MI_MASA_MARTIN 218
#define MI_MASA_BILL 219

#define MI_NPC_RHINE 220
#define MI_NPC_STIMA 221
#define MI_NPC_PHACHAM 222
#define MI_MADA_ROOCKY 223
#define MI_MADA_OLLIEN 224
#define MI_MADA_HAVEN 225
#define MI_MADA_CHITLLER 226
#define MI_MADA_BOLPOR 227
#define MI_MADA_ALMANI 228
#define MI_MADA_ACHABEN 229
#define MI_MADA_ESHYLOP 230
#define MI_MADA_REMINE 231

#define MI_MADA_UNKNOWN 232
#define MI_MADA_AGENT 233

#define MI_MADA_GUARDIAN 234
#define MI_MAMA_ANCIMYS 235

#define MI_MAFL_SANTA 236
#define MI_MAFL_PRIST 237
#define MI_NPC_CHEETOS 238
#define MI_NPC_COLABEAR 239

#define MI_FLBYRIGEN1 300
#define MI_FLBYRIGEN2 301
#define MI_FLBYRIGEN3 302
#define MI_FLBYRIGEN4 303
#define MI_MOTHBEE1 304
#define MI_MOTHBEE2 305
#define MI_MOTHBEE3 306
#define MI_MOTHBEE4 307
#define MI_ROCKEPELLER1 308
#define MI_ROCKEPELLER2 309
#define MI_ROCKEPELLER3 310
#define MI_ROCKEPELLER4 311

#define MI_GIGGLEBOX1 400
#define MI_GIGGLEBOX2 401
#define MI_GIGGLEBOX3 402
#define MI_GIGGLEBOX4 403
#define MI_GARBAGEPIDER1 404
#define MI_GARBAGEPIDER2 405
#define MI_GARBAGEPIDER3 406
#define MI_GARBAGEPIDER4 407
#define MI_RAMPAGEBUTCHER1 408
#define MI_RAMPAGEBUTCHER2 409
#define MI_RAMPAGEBUTCHER3 410
#define MI_RAMPAGEBUTCHER4 411

#define MI_GREEMONG1 500
#define MI_GREEMONG2 501
#define MI_GREEMONG3 502
#define MI_GREEMONG4 503
#define MI_DRILLER1 504
#define MI_DRILLER2 505
#define MI_DRILLER3 506
#define MI_DRILLER4 507
#define MI_STEELKNIGHT1 508
#define MI_STEELKNIGHT2 509
#define MI_STEELKNIGHT3 510
#define MI_STEELKNIGHT4 511
#define MI_ELDERGUARD1 512
#define MI_ELDERGUARD2 513
#define MI_ELDERGUARD3 514
#define MI_ELDERGUARD4 515
#define MI_CRANEMACHINERY1 516
#define MI_CRANEMACHINERY2 517
#define MI_CRANEMACHINERY3 518
#define MI_CRANEMACHINERY4 519
#define MI_POPCRANK1 520
#define MI_POPCRANK2 521
#define MI_POPCRANK3 522
#define MI_POPCRANK4 523
#define MI_PEAKYTURTLE1 524
#define MI_PEAKYTURTLE2 525
#define MI_PEAKYTURTLE3 526
#define MI_PEAKYTURTLE4 527
#define MI_HOBO1 528
#define MI_HOBO2 529
#define MI_HOBO3 530
#define MI_HOBO4 531
#define MI_CLAWDOLL1 532
#define MI_CLAWDOLL2 533
#define MI_CLAWDOLL3 534
#define MI_CLAWDOLL4 535
#define MI_CARRIERBOMB1 536
#define MI_CARRIERBOMB2 537
#define MI_CARRIERBOMB3 538
#define MI_CARRIERBOMB4 539
#define MI_LEYENA1 540
#define MI_LEYENA2 541
#define MI_LEYENA3 542
#define MI_LEYENA4 543
#define MI_DUMBBULL1 544
#define MI_DUMBBULL2 545
#define MI_DUMBBULL3 546
#define MI_DUMBBULL4 547
#define MI_NUTTYWHEEL1 548
#define MI_NUTTYWHEEL2 549
#define MI_NUTTYWHEEL3 550
#define MI_NUTTYWHEEL4 551
#define MI_JACKTHEHAMMER1 552
#define MI_JACKTHEHAMMER2 553
#define MI_JACKTHEHAMMER3 554
#define MI_JACKTHEHAMMER4 555

#define MI_MINECATCHER 556
#define MI_ERONSCATCHER 557
#define MI_KRASECCATCHER 558
#define MI_GURUCATCHER 559

#define MI_NUCTUVEHICLE1 560
#define MI_NUCTUVEHICLE2 561
#define MI_NUCTUVEHICLE3 562
#define MI_NUCTUVEHICLE4 563
#define MI_RISEM1 564
#define MI_RISEM2 565
#define MI_RISEM3 566
#define MI_RISEM4 567
#define MI_SYLIACA1 568
#define MI_SYLIACA2 569
#define MI_SYLIACA3 570
#define MI_SYLIACA4 571
#define MI_ZOMBIGER1 572
#define MI_ZOMBIGER2 573
#define MI_ZOMBIGER3 575
#define MI_ZOMBIGER4 576

#define MI_FLYBAT1 577
#define MI_FLYBAT2 578
#define MI_FLYBAT3 579
#define MI_FLYBAT4 580
#define MI_BUCROW1 581
#define MI_BUCROW2 582
#define MI_BUCROW3 583
#define MI_BUCROW4 584
#define MI_SCOPICON1 585
#define MI_SCOPICON2 586
#define MI_SCOPICON3 587
#define MI_SCOPICON4 588
#define MI_TRANGFOMA1 589
#define MI_TRANGFOMA2 590
#define MI_TRANGFOMA3 591
#define MI_TRANGFOMA4 592

#define MI_DAMAGETEST 593

#define MI_WATANGKA1 594
#define MI_WATANGKA2 595
#define MI_WATANGKA3 596
#define MI_WATANGKA4 597
#define MI_IREN1 598
#define MI_IREN2 599
#define MI_IREN3 600
#define MI_IREN4 601
#define MI_BOO1 602
#define MI_BOO2 603
#define MI_BOO3 604
#define MI_BOO4 605
#define MI_LUIA1 606
#define MI_LUIA2 607
#define MI_LUIA3 608
#define MI_LUIA4 609
#define MI_GLAPHAN1 610
#define MI_GLAPHAN2 611
#define MI_GLAPHAN3 612
#define MI_GLAPHAN4 613
#define MI_SHUHAMMA1 614
#define MI_SHUHAMMA2 615
#define MI_SHUHAMMA3 616
#define MI_SHUHAMMA4 617
#define MI_NAUTREPY1 618
#define MI_NAUTREPY2 619
#define MI_NAUTREPY3 620
#define MI_NAUTREPY4 621
#define MI_GRRR1 622
#define MI_GRRR2 623
#define MI_GRRR3 624
#define MI_GRRR4 625
#define MI_ANTIQUERY1 626
#define MI_ANTIQUERY2 627
#define MI_ANTIQUERY3 628
#define MI_ANTIQUERY4 629
#define MI_MUSHPOIE1 630
#define MI_MUSHPOIE2 631
#define MI_MUSHPOIE3 632
#define MI_MUSHPOIE4 633
#define MI_MUFFRIN1 634
#define MI_MUFFRIN2 635
#define MI_MUFFRIN3 636
#define MI_MUFFRIN4 637
#define MI_HOPPRE1 638
#define MI_HOPPRE2 639
#define MI_HOPPRE3 640
#define MI_HOPPRE4 641
#define MI_GONGURY1 642
#define MI_GONGURY2 643
#define MI_GONGURY3 644
#define MI_GONGURY4 645
#define MI_DUMP1 646
#define MI_DUMP2 647
#define MI_DUMP3 648
#define MI_DUMP4 649
#define MI_KERN1 650
#define MI_KERN2 651
#define MI_KERN3 652
#define MI_KERN4 653

#define MI_DUFEFERN1 654
#define MI_DUFEFERN2 655
#define MI_DUFEFERN3 656
#define MI_DUNYANGNYANG1 657
#define MI_DUNYANGNYANG2 658
#define MI_DUNYANGNYANG3 659
#define MI_DUBANG1 660
#define MI_DUBANG2 661
#define MI_DUBANG3 662

#define MI_GUARDMON1 663

#define MI_WORMVEDUQUE 664
#define MI_SERUSURIEL 665
#define MI_VICEVEDUQUE 666

#define MI_GUARDIAN 667
#define MI_CHAOGUARDIAN 668
#define MI_NPC_REWARD 669

#define MI_PK_WAGSAAC 670
#define MI_PK_MRPUMPKIN 671
#define MI_PK_GIGGLEBOX 672
#define MI_PK_HOBO 673
#define MI_PK_CARDPUPPET 674
#define MI_PK_BASQUE 675
#define MI_PK_LEYENA 676
#define MI_PK_STEELKNIGHT 677
#define MI_PK_VOLT 678
#define MI_PK_GARBAGEPIDER 679
#define MI_PK_GREEMONG 680
#define MI_PK_HOPPRE 681
#define MI_PK_IREN 682
#define MI_PK_WATANGKA 683
#define MI_PK_LUIA 684
#define MI_PK_SHUHAMMA 685
#define MI_PK_GLAPHAN 686

#define MI_SHURAITURE 687

#define MI_REN 688
#define MI_SISIF 689
#define MI_RUBO 690

#define MI_DU_DKKEAKOON1 691
#define MI_DU_DKKEAKOON2 692
#define MI_DU_DKKEAKOON3 693
#define MI_DU_DKKEAKOON4 694
#define MI_DU_DKKEAKOON5 695
#define MI_DU_DKKEAKOON6 696
#define MI_DU_DKKEAKOON7 697
#define MI_DU_DKKEAKOON8 698
#define MI_DU_DKKEAKOON9 699
#define MI_DU_DKKEAKOON10 700
#define MI_DU_DKKEAKOON11 701
#define MI_DU_DKKEAKOON12 702
#define MI_DU_DKROACHFL1 703
#define MI_DU_DKROACHFL2 704
#define MI_DU_DKROACHFL3 705

#define MI_DU_DKTRILLIPY1 707
#define MI_DU_DKTRILLIPY2 708
#define MI_DU_DKTRILLIPY3 709
#define MI_DU_DKTRILLIPY4 710
#define MI_DU_DKTRILLIPY5 711
#define MI_DU_DKTRILLIPY6 712
#define MI_DU_DKKIMERADON1 713
#define MI_DU_DKKIMERADON2 714
#define MI_DU_METEONYKER 715
#define MI_DU_DKROACHFL5 716
#define MI_DU_DKROACHFL6 717
#define MI_DU_DKROACHFL4 718

#define MI_PK_FEFERN 719

#define MI_PET_LAWOLF 720
#define MI_PET_AIBATT 721
#define MI_PET_LEYENA 722
#define MI_PET_LUIA 723
#define MI_PET_CAT01 724
#define MI_PET_DOG01 725
#define MI_PET_SOCCERBALL 726
#define MI_PET_COLABEAR 727
#define MI_PET_PENGUIN 728
#define MI_PET_IGUANA 729
#define MI_PET_COBRA 730

#define MI_PET_EGG 731
#define MI_PET_WHITETIGER01 732
#define MI_PET_WHITETIGER01_1 733
#define MI_PET_BARBARYLION01 734
#define MI_PET_BARBARYLION01_1 735
#define MI_PET_RABBIT02 736
#define MI_PET_RABBIT02_1 737
#define MI_PET_DRAGON01 738
#define MI_PET_DRAGON01_1 739
#define MI_PET_UNICORN01 740
#define MI_PET_UNICORN01_1 741
#define MI_PET_NINEFOX01 742
#define MI_PET_NINEFOX01_1 743
#define MI_PET_EAGLE01 744
#define MI_PET_EAGLE01_1 745

#define MI_PET_FROG 746
#define MI_PET_HAMBURGER 747
#define MI_PET_TURTLE 748

#define MI_PET_LASTINDEX 749

#define MI_CHANER 750
#define MI_BABARI 751
#define MI_SEIDO 752

#define MI_DU_METEONYKER2 753
#define MI_DU_METEONYKER3 754
#define MI_DU_METEONYKER4 755

#define MI_RBANG1 800

#define MI_VIOLMAGICION 811
#define MI_VIOLMAGICION2 812

#define MI_ORGANIGOR 813
#define MI_GANGARD 814
#define MI_HADESEOR 815

#define MI_VIOLMAGICION3 816

#define MI_RBANG2 817
#define MI_LORDBANG 818

#define MI_HAMMERKICK 819
#define MI_ANTTURTLE 820

#define MI_EMERALDMANTIS 821

#define MI_LOADCLOCKWORK 822
#define MI_BRIGADIER 823
#define MI_LIEUTENANT 824
#define MI_SPIKETAIL 825
#define MI_GLYPHAXZ 826

#define MI_LBHANOYAN 827

#define MI_KIMERADON1 828
#define MI_KIMERADON2 829
#define MI_KIMERADON3 830
#define MI_KIMERADON4 831
#define MI_KIMERADON5 832
#define MI_BEARNUCKY1 833
#define MI_BEARNUCKY2 834
#define MI_BEARNUCKY3 835
#define MI_BEARNUCKY4 836
#define MI_BEARNUCKY5 837
#define MI_MUFFRIN6 838
#define MI_POPCRANK5 839

#define MI_MOMYORN 840
#define MI_KIDLER 841
#define MI_SHAKALPION 842
#define MI_HOIREN 843
#define MI_HUNTERX 844
#define MI_KYNSY 845
#define MI_CLOCKS 846

#define MI_IBLCRASHER 855
#define MI_IBLPOISONER 856
#define MI_IBLWRECKER 857
#define MI_IBLDOZER 858
#define MI_IBLPUPPET 859
#define MI_IBLTAKER 860

#define MI_IBLGUARDER 861
#define MI_IBLQUAKER 862
#define MI_IBLMUCILAGER 863
#define MI_IBLLINESS 864

#define MI_IBLREDOTEM 865
#define MI_IBLBLACKOTEM 866
#define MI_IBLBOXTER 867
#define MI_IBLDANDISHER 868

#define MI_ANGELRED 869
#define MI_ANGELBLUE 870
#define MI_ANGELGREEN 871
#define MI_ANGELWHITE 872

#define MI_POSTBOX 873

#define MI_CYCLOPSX 874

#define MI_MADA_REDROBEMAN 875
#define MI_MADA_REDROBEGIRL 876

#define MI_NPC_DEALER 877
#define MI_NPC_HAIR 878
#define MI_NPC_MAKEUP 879
#define MI_NPC_PETTAMER 880
#define MI_NPC_DANCER 881
#define MI_NPC_MISSFLYFF 882
#define MI_NPC_MRFLYFF 883

#define MI_PET_WHITETIGER01_2 884
#define MI_PET_BARBARYLION01_2 885
#define MI_PET_RABBIT02_2 886
#define MI_PET_DRAGON01_2 887
#define MI_PET_UNICORN01_2 888
#define MI_PET_NINEFOX01_2 889
#define MI_PET_EAGLE01_2 890

#define MI_NPC_PRIEST 891
#define MI_NPC_MISTBOY 892

#define MI_PET_CHICKEN 893
#define MI_PET_COW 894
#define MI_PET_DOG2 895
#define MI_PET_DRAGON 896
#define MI_PET_HAMSTER 897
#define MI_PET_HORSE 898
#define MI_PET_MONKEY 899
#define MI_PET_PIG 900
#define MI_PET_RABBIT 901
#define MI_PET_SHEEP 902
#define MI_PET_TIGER 903
#define MI_NPC_YETI01 904
#define MI_NPC_YETI02 905
#define MI_NPC_AUGOO01 906
#define MI_NPC_AUGOO02 907
#define MI_NPC_SADKING01 908
#define MI_NPC_SADKING02 909
#define MI_NPC_WAFORU 910
#define MI_NPC_MAMMOTH01 911
#define MI_NPC_MAMMOTH02 912
#define MI_NPC_COLLECT 913
#define MI_PET_CARDPUPPET1 914
#define MI_PET_MIA1 915
#define MI_PET_DRAGON1 916
#define MI_EVENT01 917
#define MI_EVENT02 918
#define MI_EVENT03 919
#define MI_EVENT04 920
#define MI_NPC_TARGET 921

#define MI_HARPY01 922
#define MI_HARPY02 923
#define MI_HARPY03 924
#define MI_HARPY04 925

#define MI_POLEVIK01 926
#define MI_POLEVIK02 927
#define MI_POLEVIK03 928
#define MI_POLEVIK04 929

#define MI_ABRAXAS01 930
#define MI_ABRAXAS02 931
#define MI_ABRAXAS03 932
#define MI_ABRAXAS04 933

#define MI_HAG01 934
#define MI_HAG02 935
#define MI_HAG03 936
#define MI_HAG04 937

#define MI_THOTH01 938
#define MI_THOTH02 939
#define MI_THOTH03 940
#define MI_THOTH04 941

#define MI_KHNEMU01 942
#define MI_KHNEMU02 943
#define MI_KHNEMU03 944
#define MI_KHNEMU04 945

#define MI_DANTALIAN01 946
#define MI_DANTALIAN02 947
#define MI_DANTALIAN03 948
#define MI_DANTALIAN04 949

#define MI_GANESA01 950
#define MI_GANESA02 951
#define MI_GANESA03 952
#define MI_GANESA04 953

#define MI_ASURA01 954
#define MI_ASURA02 955
#define MI_ASURA03 956
#define MI_ASURA04 957

#define MI_CAITSITH01 958
#define MI_CAITSITH02 959
#define MI_CAITSITH03 960
#define MI_CAITSITH04 961

#define MI_IMP01 962
#define MI_IMP02 963
#define MI_IMP03 964

#define MI_LUCIFER01 965

#define MI_RANGDA01 966
#define MI_RANGDA02 967
#define MI_RANGDA03 968
#define MI_RANGDA04 969

#define MI_PET_YETI 970

#define MI_CAITSITH04_1 971
#define MI_HARPY04_1 972
#define MI_POLEVIK04_1 973
#define MI_ABRAXAS04_1 974
#define MI_HAG04_1 975
#define MI_THOTH04_1 976
#define MI_KHNEMU04_1 977
#define MI_DANTALIAN04_1 978
#define MI_GANESA04_1 979
#define MI_ASURA04_1 980

#define MI_NPC_KNIGHT01 981
#define MI_NPC_KNIGHT02 982
#define MI_NPC_SECRETARY 983
#define MI_NPC_SNOWGIRL 984

#define MI_NPC_RAINBOWNPC01 985
#define MI_NPC_RAINBOWNPC02 986
#define MI_NPC_RAINBOWNPC03 987
#define MI_NPC_RAINBOWNPC04 988
#define MI_NPC_RAINBOWNPC05 989
#define MI_NPC_RAINBOWNPC06 990
#define MI_NPC_RAINBOWNPC07 991
#define MI_NPC_RAINBOWSTART 992

#define MI_KINGSTER01 993
#define MI_KINGSTER02 994
#define MI_KINGSTER03 995

#define MI_KRAKEN01 996
#define MI_KRAKEN02 997
#define MI_KRAKEN03 998

#define MI_CREPER01 999
#define MI_CREPER02 1000
#define MI_CREPER03 1001

#define MI_NAGA01 1002
#define MI_NAGA02 1003
#define MI_NAGA03 1004

#define MI_ATROX01 1005
#define MI_ATROX02 1006
#define MI_ATROX03 1007

#define MI_OKEAN01 1008
#define MI_OKEAN02 1009
#define MI_OKEAN03 1010

#define MI_TAIGA01 1011
#define MI_TAIGA02 1012
#define MI_TAIGA03 1013

#define MI_DORIAN01 1014
#define MI_DORIAN02 1015
#define MI_DORIAN03 1016

#define MI_MEREL01 1017
#define MI_MEREL02 1018
#define MI_MEREL03 1019

#define MI_NPC_MINIDOOR01 1020

#define MI_MAFL_PATROL 1021
#define MI_MAFL_GUILDWAR01 1022
#define MI_MAFL_GUILDWAR02 1023
#define MI_MAFL_GUILDWAR03 1024
#define MI_MAFL_GUILDWAR04 1025
#define MI_MAFL_ARENA 1026
#define MI_MAFL_MAYOR 1027

#define MI_NPC_CHARLIE 1028

#define MI_NPC_SNOWMAN01 1029
#define MI_NPC_SNOWMAN02 1030

#define MI_PET_BULLDOG 1031
#define MI_PET_GHOST 1032
#define MI_PET_HAETAE 1033
#define MI_PET_OWL 1034
#define MI_PET_RAGDOLL 1035
#define MI_PET_ROBOT 1036
#define MI_PET_BANG1 1037
#define MI_PET_PANDA 1038
#define MI_PET_TAIGA01 1039
#define MI_PET_TAIGA02 1040
#define MI_PET_TAIGA03 1041

#define MI_Crohell01 1042
#define MI_Crohell02 1043
#define MI_Crohell03 1044
#define MI_Crohell04 1045
#define MI_Frinker01 1046
#define MI_Frinker02 1047
#define MI_Frinker03 1048
#define MI_Frinker04 1049
#define MI_Toadrin01 1050
#define MI_Toadrin02 1051
#define MI_Toadrin03 1052
#define MI_Toadrin04 1053
#define MI_Hatsalra01 1054
#define MI_Hatsalra02 1055
#define MI_Hatsalra03 1056
#define MI_Hatsalra04 1057
#define MI_Berken01 1058
#define MI_Berken02 1059
#define MI_Berken03 1060
#define MI_Berken04 1061

#define MI_PRICKANT01 1062
#define MI_PRICKANT02 1063
#define MI_PRICKANT03 1064
#define MI_PRICKANT04 1065
#define MI_CRIPESCENTIPEDE01 1066
#define MI_CRIPESCENTIPEDE02 1067
#define MI_CRIPESCENTIPEDE03 1068
#define MI_CRIPESCENTIPEDE04 1069
#define MI_MAULMOUSE01 1070
#define MI_MAULMOUSE02 1071
#define MI_MAULMOUSE03 1072
#define MI_MAULMOUSE04 1073
#define MI_LYCANOS01 1074
#define MI_VEMPAIN01 1075

#define MI_PRICKANT01_1 1076
#define MI_PRICKANT02_1 1077
#define MI_PRICKANT03_1 1078
#define MI_PRICKANT04_1 1079
#define MI_CRIPESCENTIPEDE01_1 1080
#define MI_CRIPESCENTIPEDE02_1 1081
#define MI_CRIPESCENTIPEDE03_1 1082
#define MI_CRIPESCENTIPEDE04_1 1083
#define MI_MAULMOUSE01_1 1084
#define MI_MAULMOUSE02_1 1085
#define MI_MAULMOUSE03_1 1086
#define MI_MAULMOUSE04_1 1087
#define MI_LYCANOS01_1 1088
#define MI_VEMPAIN01_1 1089

#define MI_MAHA_JANO 1090
#define MI_MAHA_VESPU 1091
#define MI_MAHA_LUCA 1092
#define MI_MAHA_LASA 1093
#define MI_MAHA_RYAN 1094

#define MI_PET_PARROT 1095

#define MI_MAFL_ETE 1096
#define MI_MAFL_TONGE 1097

#define MI_PET_ROBOT01 1098

#define MI_MAFL_MISSVTN 1099

#define MI_MOCOMOCHI1 1100

#define MI_MAFL_TELEPORTER 1101

#define MI_PET_RACCON 1102

#define MI_NPC_GUILDHOUSE_DOOR01 1103

#define MI_NPC_SELIA 1104
#define MI_NPC_ICINIS 1105

#define MI_SKELWOLF 1106
#define MI_SKELSWORD 1107
#define MI_SKELSPEAR 1108
#define MI_SKELMAGE 1109
#define MI_SKELASSASSIN 1110
#define MI_SKELFIGHTER 1111
#define MI_SKELGENERAL 1112
#define MI_SKELGRIFFIN 1113
#define MI_SKELLEADER 1114
#define MI_SKELSPAIN 1115
#define MI_SKELSHAMEN 1116
#define MI_SKELRIDER 1117
#define MI_SKELDEVIL 1118

#define MI_MAFL_TELEPORTER_3VAT 1119

#define MI_PET_SANTACLAUS 1120
#define MI_PET_SNOWMAN02 1121

#define MI_NPC_VALLOSIA 1122
#define MI_NPC_ROLINE 1123

#define MI_PET_RICAN 1124

#define MI_DREADSTONE01 1125
#define MI_DREADSTONE02 1126
#define MI_DREADSTONE03 1127
#define MI_DREADSTONE04 1128
#define MI_DREADSTONE05 1129
#define MI_DREADSTONE06 1130

#define MI_BIGCOBRA01 1131
#define MI_BIGVARIETYCOBRA01 1132
#define MI_BIGRACCOON01 1133
#define MI_BIGSOLDIERRACCOON01 1134
#define MI_SOLDIERTANGZ01 1135
#define MI_SHAMANWUTANGKA01 1136
#define MI_BESIBIGFOOT01 1137

#define MI_BIGCOBRA02 1138
#define MI_BIGVARIETYCOBRA02 1139
#define MI_BIGRACCOON02 1140
#define MI_BIGSOLDIERRACCOON02 1141
#define MI_SOLDIERTANGZ02 1142
#define MI_SHAMANWUTANGKA02 1143
#define MI_BESIBIGFOOT02 1144

#define MI_RUSTIACRASHGATE01 1145
#define MI_RUSTIACRASHGATE02 1146
#define MI_RUSTIACRASHGATE03 1147
#define MI_RUSTIACRASHGATE04 1148

#define MI_PET_DESERTFOX 1149
#define MI_PET_DOBERMAN 1150
#define MI_PET_ICEQUEEN 1151
#define MI_PET_WRONGKOALA 1152

#define MI_NPC_LEPRECHAUN 1153

#define MI_PET_CHESHIRECAT 1154
#define MI_PET_WHITERABBIT 1155
#define MI_PET_WHITETIGER02 1156

#define MI_RABBITGUARDER01 1157
#define MI_TUTTLESWORDER01 1158
#define MI_TUTTLEFIGHTER01 1159
#define MI_TUTTLEASSASSIN01 1160
#define MI_TUTTLESPEAR01 1161
#define MI_TUTTLEAXE01 1162
#define MI_TUTTLEKING01 1163

#define MI_MaFl_Babario 1164
#define MI_PET_SKEL01 1165

#define MI_PET_MOCOMOCI 1166
#define MI_PET_DANCER 1167
#define MI_CLOCKWORKBUTLER01 1168

#define MI_STATUE 1169
#define MI_SPIRITTULA 1170
#define MI_SPIRITUMTULA 1171
#define MI_SPIRITOBNIS 1172
#define MI_SPIRITBAGNIS 1173
#define MI_BEGUARDIAN 1174
#define MI_BEHEMOTH 1175
#define MI_BEHESTATUE01 1210

#define MI_PUPPETWOLF 1176
#define MI_RYSENTRY 1177
#define MI_RYGUARD 1178
#define MI_RYARCHER 1179
#define MI_RYMAGI 1180
#define MI_RYWARRIOR 1181
#define MI_RYBARGA 1182

#define MI_NPCBULLSFESTIVAL 1183
#define MI_DUMBBULL5 1184

#define MI_MaEw_RUIDAN 1185
#define MI_MaEw_HUNTRANG 1186
#define MI_MaEw_MIORANG 1187
#define MI_MaEw_MAWRANG 1188
#define MI_MaEw_KANRANG 1189
#define MI_MaEw_RALBADAN 1190
#define MI_MaEw_RELGANTUS 1191
#define MI_MaEw_MIRIUN 1192
#define MI_MaEw_KARANG 1193
#define MI_MaEw_KURANG 1194
#define MI_MaEw_HEIRANG 1195
#define MI_MaEw_ARANG 1196
#define MI_MaEw_MAURANG 1197
#define MI_MaEw_BATO 1198
#define MI_MaEw_MEIALUN 1199
#define MI_MaEw_HAWRANG 1200
#define MI_MaEw_RUTAM 1201
#define MI_MaEw_RAYA 1202
#define MI_MaEw_HARLIE 1203
#define MI_MaEw_RODELLA 1204
#define MI_MaEw_EPIE 1205
#define MI_MaEw_HURI 1206
#define MI_MaEw_GELGA 1207
#define MI_MaEw_RUOBORU 1208
#define MI_MaEw_CHEIRNAG 1212
#define MI_MaEw_EMBLUM 1213
#define MI_MaEw_CARL 1214
#define MI_MaEw_BRANKA 1215
#define MI_MaEw_ISILIS 1216
#define MI_MaEw_ROMAIN 1217
#define MI_MaEw_GUARDIAN01 1218
#define MI_MaEw_GUARDIAN02 1219

#define MI_SPAINDRAGON01 1209

#define MI_MaFl_ROMINA 1211

#define MI_PET_SMELTPIYO 1220
#define MI_PET_SMELTUDI 1221

#define MI_PET_SHEEP1 1222

#define MI_PET_SMELTUDI01 1274

#define MI_MaEw_MEWRANG 1275

#define MI_MIA5 1276
#define MI_CARDPUPPET5 1277
#define MI_RANGDA05 1278

#define MI_PINATA 1279

#define MI_NPCDEMIAN 1280

#define MI_PET_CHAMELEON 1281

#define MI_LIGHT 1282
#define MI_DARK 1283

#define MI_MaSa_ROA 1284
#define MI_MaEw_TROY 1285

#define MI_MaFl_GUILDHOUSE_DOOR_01 1286
#define MI_MaFl_GUILDHOUSE_DOOR_02 1287
#define MI_MaFl_GUILDHOUSE_DOOR_03 1288
#define MI_MaFl_GUILDHOUSE_DOOR_04 1289
#define MI_MaFl_GUILDHOUSE_DOOR_05 1290

#define MI_MaSa_GUILDHOUSE_DOOR_01 1291
#define MI_MaSa_GUILDHOUSE_DOOR_02 1292
#define MI_MaSa_GUILDHOUSE_DOOR_03 1293
#define MI_MaSa_GUILDHOUSE_DOOR_04 1294
#define MI_MaSa_GUILDHOUSE_DOOR_05 1295
#define MI_MaSa_GUILDHOUSE_DOOR_06 1296
#define MI_MaSa_GUILDHOUSE_DOOR_07 1297
#define MI_MaSa_GUILDHOUSE_DOOR_08 1298
#define MI_MaSa_GUILDHOUSE_DOOR_09 1299

#define MI_MaEw_GUILDHOUSE_DOOR_01 1300
#define MI_MaEw_GUILDHOUSE_DOOR_02 1301
#define MI_MaEw_GUILDHOUSE_DOOR_03 1302

#define MI_NPC_ROMERO 1303
#define MI_NPC_FRANKENSTEIN 1304

#define MI_MaFl_GUILDHOUSE_NOTICE_01 1305
#define MI_MaFl_GUILDHOUSE_NOTICE_02 1306
#define MI_MaFl_GUILDHOUSE_NOTICE_03 1307
#define MI_MaFl_GUILDHOUSE_NOTICE_04 1308
#define MI_MaFl_GUILDHOUSE_NOTICE_05 1309

#define MI_MaSa_GUILDHOUSE_NOTICE_01 1310
#define MI_MaSa_GUILDHOUSE_NOTICE_02 1311
#define MI_MaSa_GUILDHOUSE_NOTICE_03 1312
#define MI_MaSa_GUILDHOUSE_NOTICE_04 1313
#define MI_MaSa_GUILDHOUSE_NOTICE_05 1314
#define MI_MaSa_GUILDHOUSE_NOTICE_06 1315
#define MI_MaSa_GUILDHOUSE_NOTICE_07 1316
#define MI_MaSa_GUILDHOUSE_NOTICE_08 1317
#define MI_MaSa_GUILDHOUSE_NOTICE_09 1318

#define MI_MaEw_GUILDHOUSE_NOTICE_01 1319
#define MI_MaEw_GUILDHOUSE_NOTICE_02 1320
#define MI_MaEw_GUILDHOUSE_NOTICE_03 1321

#define MI_BUFFPONG 1322

#define MI_MZOMBIE 1323
#define MI_FZOMBIE 1324

#define MI_PET_LITTLEZOMBIE 1325

#define MI_GUILD_DOOR 1326

#define MI_NPC_BROOKS 1327

#define MI_NPC_ATMA 1328

#define MI_NPC_JAPCHANGE 1329

#define MI_NMZOMBIE 1330
#define MI_NFZOMBIE 1331

#define MI_MIDDLE_GUILDHOUSE_DOOR_01 1332
#define MI_MIDDLE_GUILDHOUSE_DOOR_02 1333
#define MI_MIDDLE_GUILDHOUSE_DOOR_03 1334

#define MI_TGLUIA01 1335

#define MI_PET_REDPAANG 1336

#define MI_MaEw_RUKAS 1337
#define MI_MaEw_PiINANOCO 1338
#define MI_MaEw_RAUNDAS 1339

#define MI_BASILISK 1340
#define MI_RUGALHEAT01 1341
#define MI_RUGALHEAT02 1342
#define MI_RUGALKUMA 1343
#define MI_RUGALRIM 1344
#define MI_RUGALRIMA 1345
#define MI_RUGALWIND02 1346
#define MI_KALGASAKIN01 1347
#define MI_KALGASBABY01 1348
#define MI_KALGASBALT01 1349
#define MI_KALGASFLY01 1350
#define MI_KALGASKUMA01 1351
#define MI_KALGASLESSER01 1352
#define MI_KALGASRIMA01 1353
#define MI_KALGASBOSS 1354
#define MI_RUGALKUM 1355
#define MI_RUGALWIN 1356

#define MI_MaEw_IDELRUNA 1357
#define MI_MaEw_GIGAKA 1358

#define MI_COLOBANG 1359
#define MI_COLOWAGJAK 1360
#define MI_COLOREDMANTIS 1361
#define MI_COLOJACKTHEHAMMER 1362
#define MI_COLOBERKRO 1363
#define MI_COLOHOWBOW 1364
#define MI_COLOROKEPELER 1365
#define MI_COLOSTIMEWORK 1366
#define MI_COLOGRREUNG 1367
#define MI_COLOGRIMONG 1368
#define MI_COLOLUIA 1369
#define MI_COLOGONGRI 1370
#define MI_COLOKEREUN 1371
#define MI_COLOMATEONIKER 1372
#define MI_COLOCYCLOPSX 1373

#define MI_COLODIEOPNIS 1374
#define MI_COLORACCOUN 1375
#define MI_COLOMATEONIKER01 1376
#define MI_COLOCLOCKWORK 1377
#define MI_COLOBIGFOOT 1378
#define MI_COLOLUCIFER 1379
#define MI_COLOLYCANOS 1380
#define MI_COLOSOULVAMPAIN 1381
#define MI_COLOSKELGENERAL 1382
#define MI_COLOTUTTLEKING 1383
#define MI_COLORYCANBARGA 1384
#define MI_COLOANGRYBEHEMOTH 1385
#define MI_COLOBASILISK 1386
#define MI_COLOKALGAS 1387
#define MI_COLOSKELDEVIL 1388

#define MI_PET_REDSNOWMAN 1389

#define MI_COLOBANG_1 1391
#define MI_COLOWAGJAK_1 1392
#define MI_COLOREDMANTIS_1 1393
#define MI_COLOJACKTHEHAMMER_1 1394
#define MI_COLOBERKRO_1 1395
#define MI_COLOHOWBOW_1 1396
#define MI_COLOROKEPELER_1 1397
#define MI_COLOSTIMEWORK_1 1398
#define MI_COLOGRREUNG_1 1399
#define MI_COLOGRIMONG_1 1400
#define MI_COLOLUIA_1 1401
#define MI_COLOGONGRI_1 1402
#define MI_COLOKEREUN_1 1403
#define MI_COLOMATEONIKER_1 1404
#define MI_COLOCYCLOPSX_1 1405

#define MI_COLODIEOPNIS_1 1406
#define MI_COLORACCOUN_1 1407
#define MI_COLOMATEONIKER01_1 1408
#define MI_COLOCLOCKWORK_1 1409
#define MI_COLOBIGFOOT_1 1410
#define MI_COLOLUCIFER_1 1411
#define MI_COLOLYCANOS_1 1412
#define MI_COLOSOULVAMPAIN_1 1413
#define MI_COLOSKELGENERAL_1 1414
#define MI_COLOTUTTLEKING_1 1415
#define MI_COLORYCANBARGA_1 1416
#define MI_COLOANGRYBEHEMOTH_1 1417
#define MI_COLOBASILISK_1 1418
#define MI_COLOKALGAS_1 1419
#define MI_COLOSKELDEVIL_1 1420

#define MI_KALGASEGG01 1421
#define MI_KALGASSTELE01 1422
#define MI_KALGASSTELE02 1423

#define MI_BURR 1424
#define MI_SAPHYRYAN 1425
#define MI_GRAYEARL 1426

#define MI_PET_SNOWMAN01 1427
#define MI_PET_CHRISTMASFAIRY 1428
#define MI_PET_CHRISTMASDEER 1429

#define MI_EVEROCKEPELLER 1430
#define MI_EVEMOTHBEE 1431

#define MI_GPOTATO01 1432
#define MI_GPOTATO02 1436

#define MI_EVEKALGASBABY 1433
#define MI_PET_KIMPD 1434

#define MI_NPC_KIMPD 1435
#define MI_PET_CUTEBEAR 1437
#define MI_PET_SMELTCOBI 1438
#define MI_PET_SMELTPIERCE 1439

#define MI_LOVETHIEF 1440

#define MI_PET_NEWYEARRABBIT 1441

#define MI_MAFL_DONATION 1442

#define MI_PET_DEVILTOY 1443

#define MI_PET_TADDYBEAR 1444

#define MI_SHIPHARPINEES 1445
#define MI_SHIPMOUGUS 1446
#define MI_SHIPWINGMOUGUS 1447
#define MI_SHIPMESPI 1448
#define MI_SHIPWINDMESPI 1449
#define MI_SHIPREDHARPY 1450
#define MI_SHIPBLUEHARPY 1451
#define MI_SHIPHIPPOGRIPH 1452

#define MI_DREAMFLAME01 1453
#define MI_DREAMRAPRA01 1454
#define MI_DREAMOLDRUT01 1455
#define MI_DREAMMINIMUSHU01 1456
#define MI_DREAMLADYBLUM01 1457
#define MI_DREAMNIGHTMIST01 1458
#define MI_DREAMQEEN01 1459

#define MI_HERNSHARK01 1460
#define MI_HERNMERMAN01 1461
#define MI_HERNMERMAID01 1462
#define MI_HERNTURTLE01 1463
#define MI_HERNMERMAN02 1464
#define MI_HERNMERMAID02 1465
#define MI_HERNSIREN01 1466
#define MI_HERNKRAKEN01 1467

#define MI_SHIPHARPINEES_1 1468
#define MI_SHIPMOUGUS_1 1469
#define MI_SHIPWINGMOUGUS_1 1470
#define MI_SHIPMESPI_1 1471
#define MI_SHIPWINDMESPI_1 1472
#define MI_SHIPREDHARPY_1 1473
#define MI_SHIPBLUEHARPY_1 1474
#define MI_SHIPHIPPOGRIPH_1 1475

#define MI_DREAMFLAME01_1 1476
#define MI_DREAMRAPRA01_1 1477
#define MI_DREAMOLDRUT01_1 1478
#define MI_DREAMMINIMUSHU01_1 1479
#define MI_DREAMLADYBLUM01_1 1480
#define MI_DREAMNIGHTMIST01_1 1481
#define MI_DREAMQEEN01_1 1482

#define MI_HERNSHARK01_1 1483
#define MI_HERNMERMAN01_1 1484
#define MI_HERNMERMAID01_1 1485
#define MI_HERNTURTLE01_1 1486
#define MI_HERNMERMAN02_1 1487
#define MI_HERNMERMAID02_1 1488
#define MI_HERNSIREN01_1 1489
#define MI_HERNKRAKEN01_1 1490

#define RI_TRIGGER 10
#define RI_ATTRIBUTE 11
#define RI_BEGIN 12
#define RI_REVIVAL 13
#define RI_STRUCTURE 14
#define RI_PLACE 15

#define MI_PET_BEHEMOTH 1501
#define MI_PET_TUTTLEKING01 1502
#define MI_PET_RYBARGA 1503
#define MI_PET_BURR 2000
#define MI_PET_VEMPAIN 1506
#define MI_PET_SKELDEVIL 1507
#define MI_PET_METEONYKER 1510
#define MI_PET_LYCANOS01_1 1532
#define MI_PET_SKELRIDER 1515
#define MI_PET_LUCIFER 1509

#define MI_AIBATT11 1663
#define MI_PUKEPUKE44 1664
#define MI_LAWOLF44 1665
#define MI_BANG44 1666
#define MI_MRPUMPKIN44 1667
#define MI_GIGGLEBOX44 1668
#define MI_TOTEMIA44 1669
#define MI_WHEELEM44 1670

#define MI_BANG_COLO 1671
#define MI_WAGSAAC_COLO 1672
#define MI_REDMANTIS_COLO 1673
#define MI_JACKTHEHAMMER_COLO 1674
#define MI_BUCROW_COLO 1675
#define MI_HOBO_COLO 1676
#define MI_ROCKEPELLER_COLO 1677
#define MI_STEAMWALKER_COLO 1678
#define MI_GREEMONG_COLO 1679
#define MI_GRRR_COLO 1680
#define MI_LUIA_COLO 1681
#define MI_GONGURY_COLO 1682
#define MI_KERN_COLO 1683

#define MI_BESIBIGFOOT_COLO 1684
#define MI_LYCANOS_COLO 1685
#define MI_LUCIFER_COLO 1686
#define MI_VEMPAIN_COLO 1687
#define MI_TUTTLEKING_COLO 1688
#define MI_SKELGENERAL_COLO 1689
#define MI_SKELDEVIL_COLO 1690
#define MI_RYBARGA_COLO 1691
#define MI_BEHEMOTH_COLO 1692

#define GLOW_NORMAL 1566
#define GLOW_RED 1567
#define XI_GEN_ITEM_SETITEM10_RED 1568

#define XI_INT_DOUBLEKILL 66706
#define XI_INT_MULTIKILL 66707
#define XI_INT_SUPERKILL 66708
#define XI_INT_ULTRAKILL 66709
#define XI_INT_AWESOME 66710

#define MI_PET_BEELZEBUBX 1700
#define MI_PET_MUSHPANG 1701
#define MI_PET_MALE 1702
#define MI_PET_FEMALE 1703
#define MI_PET_CRAN 1704
#define MI_PET_NONN 1705
#define MI_PET_GORG 1706
#define MI_PET_CLOC 1707
#define MI_PET_GREE 1708
#define MI_PET_AUGU 1709
#define MI_PET_PRIN 1710
#define MI_PET_BEAR 1711
#define MI_PET_POPC 1712
#define MI_PET_SERU 1713
#define MI_PET_GLAP 1714
#define MI_PET_KERN 1715
#define MI_PET_PANG 1716
#define MI_PET_SHIPHARPINEES 1717
#define MI_PET_SHIPWINDMESPI 1718
#define MI_PET_SHIPREDHARPY 1719
#define MI_PET_DREAMOLDRUT01 1720
#define MI_PET_DREAMMINIMUSHU01 1721
#define MI_PET_HAG04 1722
#define MI_PET_IBLDANDISHER 1723
#define MI_PET_IREN3 1724
#define MI_PET_ZOMBIGER4 1725
#define MI_PET_MRPUMPKIN4 1726
#define MI_PET_WHEELEM4 4000
#define MI_PET_CYCLOPSX 4001
#define MI_PET_RANGDA04 4002
#define MI_PET_MEREL03 4003
#define MI_PET_DU_METEONYKER2 4004
#define MI_PET_Crohell04 4005
#define MI_PET_SKELMAGE 4006
#define MI_PET_SKELASSASSIN 4007
#define MI_PET_SPIRITBAGNIS 4008
#define MI_PET_TUTTLEFIGHTER01 4009

#define MI_PET_TIGERSYNX 4020

#define MI_CRISTMASGIGLEBOX 4010

#define MI_BESIBIGFOOT03 4011
#define MI_LYCANOS01_2 4012
#define MI_LUCIFER02 4013
#define MI_VEMPAIN01_2 4014
#define MI_TUTTLEKING02 4015
#define MI_SKELGENERAL01 4016
#define MI_SKELDEVIL01 4017
#define MI_RYBARGA01 4018
#define MI_BEHEMOTH01 4019

#define MI_FARM_GREEN_01 1727
#define MI_FARM_GREEN_02 1728
#define MI_FARM_GREEN_03 1729
#define MI_FARM_GREEN_04 1730
#define MI_FARM_GREEN_05 1731
#define MI_FARM_GREEN_06 1732
#define MI_FARM_SAND_01 1733
#define MI_FARM_SAND_02 1734
#define MI_FARM_SAND_03 1735
#define MI_FARM_SAND_04 1736
#define MI_FARM_SAND_05 1737
#define MI_FARM_SAND_06 1738
#define MI_FARM_WINTER_01 1739
#define MI_FARM_WINTER_02 1740
#define MI_FARM_WINTER_03 1741
#define MI_FARM_WINTER_04 1742
#define MI_FARM_WINTER_05 1743
#define MI_FARM_WINTER_06 1744

#define XI_SETIEM_EFFECTHAND 5735

#define XI_GEN_ITEM_SETITEM031 5100
#define XI_GEN_ITEM_SETITEM041 5101
#define XI_GEN_ITEM_SETITEM051 5102
#define XI_GEN_ITEM_SETITEM061 5103
#define XI_GEN_ITEM_SETITEM071 5104
#define XI_GEN_ITEM_SETITEM081 5105
#define XI_GEN_ITEM_SETITEM091 5106
#define XI_GEN_ITEM_SETITEM101 5107

#define XI_GEN_ITEM_SETITEM032 5108
#define XI_GEN_ITEM_SETITEM042 5109
#define XI_GEN_ITEM_SETITEM052 5110
#define XI_GEN_ITEM_SETITEM062 5111
#define XI_GEN_ITEM_SETITEM072 5112
#define XI_GEN_ITEM_SETITEM082 5113
#define XI_GEN_ITEM_SETITEM092 5114
#define XI_GEN_ITEM_SETITEM102 5115

#define XI_GEN_ITEM_SETITEM033 5116
#define XI_GEN_ITEM_SETITEM043 5117
#define XI_GEN_ITEM_SETITEM053 5118
#define XI_GEN_ITEM_SETITEM063 5119
#define XI_GEN_ITEM_SETITEM073 5120
#define XI_GEN_ITEM_SETITEM083 5121
#define XI_GEN_ITEM_SETITEM093 5122
#define XI_GEN_ITEM_SETITEM103 5123

#define XI_GEN_ITEM_SETITEM034 5124
#define XI_GEN_ITEM_SETITEM044 5125
#define XI_GEN_ITEM_SETITEM054 5126
#define XI_GEN_ITEM_SETITEM064 5127
#define XI_GEN_ITEM_SETITEM074 5128
#define XI_GEN_ITEM_SETITEM084 5129
#define XI_GEN_ITEM_SETITEM094 5130
#define XI_GEN_ITEM_SETITEM104 5131

#define XI_GEN_ITEM_SETITEM035 5132
#define XI_GEN_ITEM_SETITEM045 5133
#define XI_GEN_ITEM_SETITEM055 5134
#define XI_GEN_ITEM_SETITEM065 5135
#define XI_GEN_ITEM_SETITEM075 5136
#define XI_GEN_ITEM_SETITEM085 5137
#define XI_GEN_ITEM_SETITEM095 5138
#define XI_GEN_ITEM_SETITEM105 5139

#define XI_GEN_ITEM_SETITEM036 5140
#define XI_GEN_ITEM_SETITEM046 5141
#define XI_GEN_ITEM_SETITEM056 5142
#define XI_GEN_ITEM_SETITEM066 5143
#define XI_GEN_ITEM_SETITEM076 5144
#define XI_GEN_ITEM_SETITEM086 5145
#define XI_GEN_ITEM_SETITEM096 5146
#define XI_GEN_ITEM_SETITEM106 5147

#define XI_GEN_ITEM_SETITEM037 5148
#define XI_GEN_ITEM_SETITEM047 5149
#define XI_GEN_ITEM_SETITEM057 5150
#define XI_GEN_ITEM_SETITEM067 5151
#define XI_GEN_ITEM_SETITEM077 5152
#define XI_GEN_ITEM_SETITEM087 5153
#define XI_GEN_ITEM_SETITEM097 5154
#define XI_GEN_ITEM_SETITEM107 5155

#define XI_GEN_ITEM_SETITEM038 5156
#define XI_GEN_ITEM_SETITEM048 5157
#define XI_GEN_ITEM_SETITEM058 5158
#define XI_GEN_ITEM_SETITEM068 5159
#define XI_GEN_ITEM_SETITEM078 5160
#define XI_GEN_ITEM_SETITEM088 5161
#define XI_GEN_ITEM_SETITEM098 5162
#define XI_GEN_ITEM_SETITEM108 5163

#define XI_GEN_ITEM_SETITEM039 5164
#define XI_GEN_ITEM_SETITEM049 5165
#define XI_GEN_ITEM_SETITEM059 5166
#define XI_GEN_ITEM_SETITEM069 5167
#define XI_GEN_ITEM_SETITEM079 5168
#define XI_GEN_ITEM_SETITEM089 5169
#define XI_GEN_ITEM_SETITEM099 5170
#define XI_GEN_ITEM_SETITEM109 5171

#define XI_GEN_ITEM_SETITEM0310 5172
#define XI_GEN_ITEM_SETITEM0410 5173
#define XI_GEN_ITEM_SETITEM0510 5174
#define XI_GEN_ITEM_SETITEM0610 5175
#define XI_GEN_ITEM_SETITEM0710 5176
#define XI_GEN_ITEM_SETITEM0810 5177
#define XI_GEN_ITEM_SETITEM0910 5178
#define XI_GEN_ITEM_SETITEM1010 5179

#define XI_GEN_ITEM_SETITEM0311 5180
#define XI_GEN_ITEM_SETITEM0411 5181
#define XI_GEN_ITEM_SETITEM0511 5182
#define XI_GEN_ITEM_SETITEM0611 5183
#define XI_GEN_ITEM_SETITEM0711 5184
#define XI_GEN_ITEM_SETITEM0811 5185
#define XI_GEN_ITEM_SETITEM0911 5186
#define XI_GEN_ITEM_SETITEM1011 5187

#define XI_GEN_ITEM_SETITEM0312 5188
#define XI_GEN_ITEM_SETITEM0412 5189
#define XI_GEN_ITEM_SETITEM0512 5190
#define XI_GEN_ITEM_SETITEM0612 5191
#define XI_GEN_ITEM_SETITEM0712 5192
#define XI_GEN_ITEM_SETITEM0812 5193
#define XI_GEN_ITEM_SETITEM0912 5194
#define XI_GEN_ITEM_SETITEM1012 5195

#define XI_GEN_ITEM_SETITEM1013 5196

#define XI_GEN_ITEM_SETITEM1014 5197

#define XI_GEN_ITEM_SETITEM1015 5198

#define XI_GEN_ITEM_SETITEM1016 5199

#define XI_GEN_ITEM_SETITEM1017 5200

#define XI_GEN_ITEM_SETITEM1018 5201

#define XI_GEN_ITEM_SETITEM1019 5202

#define XI_GEN_ITEM_SETITEM1020 5203

#define XI_GEN_ITEM_SETITEM1021 5204
#define XI_GEN_ITEM_SETITEM1022 5205
#define XI_GEN_ITEM_SETITEM1023 5206

#define XI_SETIEM_EFFECTHAND1 5500
#define XI_SETIEM_EFFECTHAND2 5501
#define XI_SETIEM_EFFECTHAND3 5502
#define XI_SETIEM_EFFECTHAND4 5503
#define XI_SETIEM_EFFECTHAND5 5504

#define XI_SETIEM_EFFECTHAND6 5505
#define XI_SETIEM_EFFECTHAND7 5506
#define XI_SETIEM_EFFECTHAND8 5507
#define XI_SETIEM_EFFECTHAND9 5508
#define XI_SETIEM_EFFECTHAND10 5509

#define XI_SETIEM_EFFECTHAND11 5510
#define XI_SETIEM_EFFECTHAND12 5511
#define XI_SETIEM_EFFECTHAND13 5512
#define XI_SETIEM_EFFECTHAND14 5513
#define XI_SETIEM_EFFECTHAND15 5514
#define XI_SETIEM_EFFECTHAND16 5515
#define XI_SETIEM_EFFECTHAND17 5516
#define XI_SETIEM_EFFECTHAND18 5517

#define XI_SETIEM_EFFECTHAND19 5518
#define XI_SETIEM_EFFECTHAND20 5519
#define XI_SETIEM_EFFECTHAND21 5520

#define XI_SETIEM_EFFECTHAND22 5521
#define XI_SETIEM_EFFECTHAND23 5522

#define MI_TURTLEGUARDER 1534
#define MI_RABBITFIGHTER 1535
#define MI_RABBITSWORDER 1536
#define MI_RABBITSPEAR 1537
#define MI_RABBITASSASSIN 1538
#define MI_RABBITAXE 1539
#define MI_IBLESSPUPPET 1540

#define MI_PET_FROSTHUNTER01 1541
#define MI_PET_FROSTHUNTER01_1 1542
#define MI_PET_FROSTHUNTER01_2 1543