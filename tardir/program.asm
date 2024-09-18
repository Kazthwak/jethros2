[ORG 0x00000000]
[BITS 32]
push eax
push ebx
push ecx
mov ecx, message
mov eax, 0x05
lop:
	mov ebx, [ecx]
	int 0x30
	inc ecx
	cmp ecx, end
	jle lop
pop ecx
pop ebx
pop eax
ret
message:
db "This is a really long string that contains a lot of data. I need to pad this out so that it takes up more than"
db " 512 bytes of data and it fails to work due to the fact that I am only loading a single sector from disk. This"
db " will give me a method to test it. Unfortunately it was still not long enough, so I have had to add this! I do"
db " not understand how this message is still too short, but sinxe it is, I have once again added some garbage to it"
db ". After considering it some more, I have come to the conclusion that I was miscalculating, and my message needed"
db " to be around twice as long as I thought. I have hence added an incredibly long and pointless sentence to the end"
db " of it to ensure it meets the required length. Any moment now, when I run it, I will discover that the message"
db " has overflowed and this program prints out garbage data past the end of this string, but is this string enough?"
db " I am hoping that my script will dynamically change the quantity of file loaded to account for the file size "
end:
;times(0x1000) db 0x0