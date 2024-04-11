.section .hollyhock_name, "a"
.global _hollyhock_name
.type _hollyhock_name, %object
_hollyhock_name:
.string "PIE Loader"
.size _hollyhock_name, .-_hollyhock_name

.section .hollyhock_description, "a"
.global _hollyhock_description
.type _hollyhock_description, %object
_hollyhock_description:
.string "A elf loader for pie executabels"
.size _hollyhock_deescription, .-_hollyhock_description

.section .hollyhock_author, "a"
.global _hollyhock_author
.type _hollyhock_author, %object
_hollyhock_author:
.string "QBos07"
.size _hollyhock_author, .-_hollyhock_author

.section .hollyhock_version, "a"
.global _hollyhock_version
.type _hollyhock_version, %object
_hollyhock_version:
.string "0.1"
.size _hollyhock_version, .-_hollyhock_version

.text
.global _Debug_WaitKey
.set _Debug_WaitKey, 0x80094380

.global _LCD_Refresh
.set _LCD_Refresh, 0x8003733E
