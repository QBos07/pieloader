.section .hollyhock_name
.global _hollyhock_name
.type _hollyhock_name, %object
_hollyhock_name:
.string "PIE Loader"
.section .hollyhock_description
.global _hollyhock_description
.type _hollyhock_description, %object
_hollyhock_description:
.string "A elf loader for pie executabels"
.section .hollyhock_author
.global _hollyhock_author
.type _hollyhock_author, %object
_hollyhock_author:
.string "QBos07"
.section .hollyhock_version
.global _hollyhock_version
.type _hollyhock_version, %object
_hollyhock_version:
.string "0.1"

.text
.global _Debug_WaitKey
.set _Debug_WaitKey, 0x80094380
