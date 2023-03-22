File: readme.txt
Author: Jie Zhu
----------------------

ATM
---
1a)

Defect description:
-------------------
Since cust->balance is a signed number and amount is a unsigned number, balance and the result will be casted to unsigned during
the subtract operation. Thus, if we withdraw money more than balance, the result will still be positive because of binary representation.
This issue will lead us to pass process_request even our withdraw is larger than balance. However, the balance will show negative correctly
because the value is changed to signed during the assigning step.

Test case explanation (put test case in custom_tests!):
-------------------------------------------------------
In test case, I set amount to be 222 which causes the withdraw function return true. Any amount greater than 107 and smaller or equal than 300 works can 
make balance negative and pass the test.

Recommended fix:
----------------
change unsigned int amount to int amount for parameter in withdraw function.

1b)

Defect description:
-------------------
If the username is not in the database, code will not go through the return statement. However, the function do have return value stored in rax/eax instruction
From assembler code below
0x0000000000401334 <+47>:    callq  0x4010b0 <strcmp@plt>
we know that $eax value will be based on the last call on strcmp function.

Test case explanation (put test case in custom_tests!):
-------------------------------------------------------
Since we want to modify the eax value to positive so that it can pass through return statment in lookup_by_name function, we only need to look for the last username in
database and change its last character so that we can have a positive strcmp value. In my test case, I wrote zprotzan.
Recommended fix:
----------------
remove else statement and put "return NOT_AUTHORIZED;" outside the for loop. 

1c)

Defect description:
-------------------
The issue is in read_secret_passcode function. From assembler code, we know that rsp is down 64 bytes (%rsp -= 0x40) in the stackframe 
while variable code is located up 60 bytes with 4 bytes wide (because of int type) starting from 0x3c(%rsp). Our buffer for sprintf is in the range between the 
starting position of variable code and rsp position (60 bytes total). Thus, if we exceeds 60 bytes (including null terminator) for the buffer, we will modify 
the variable code. 

Test case explanation (put test case in custom_tests!):
-------------------------------------------------------
Since BANK_DATA_DIR already has 41 characters, we can add 22 more characters (not including '\0') to overwrite the variable code. 
Thus, I have 19 'w' character and 3 'A' character as ACCOUNTNUM. "AAA" will overwrite the negative variable code starting from least significant bit.
We only need 3 'A' because the null terminator will overwrite the most significant bit with 0 which can make variable code to positive number.
After convert 'A' from least significant bit to the left, I got 4276545 which will be assigned to variable saved_code, and 4276545 will be my last argument in command line.
Recommended fix:
----------------
Add if statement in lookup_by_number to make sure the length of filename is less than 22 or we can change the sizeof(filename) to strlen(filename) in line 89

Dataset Aggregation
----

1a) Windom Earle is probably who made multiple large transactions
1b) Donna Hayward and Dale Cooper
2) I first check the withdraw activity of the identifier in transactions database. If his/her activity matches what I am looking for, I then search through online social network
   from search_checkins program. I mainly look for the person whose activity time at Stanford Bank ATM is similar as his withdraw activity on that day. Finally, when I get the 
   name from All check-ins, I search the name through the program intentionally to confirm his activity.
3) It is better to encrpt the datatime and amount of withdraw in the bank's logfile so that only specific groups in the bank can understant the contents. 
   Another way is to add some inconsequential noise to mislead some information in the logfile.
4) The information that can be aggregated here is a violation of privacy because losing control how our private information is shared with ours.
   From privacy as control of information model, controlling over personal data being collected is important to us.

SecureVault
----
1) I use break at trigger_alarm first. When I trace through assembler code in gdb, I will stop typing nexti if code jumps to trigger_alarm and look for the reason causing it.

2) The hax value is the address of a string (0x403608). It can move to $edi because its msb position is less than 32. The movement purpose is to compare with the input user typed. 

3) jg checks the previous flag for xor instruction, if it is SF and OF, jg branch taken. Thus, as long as we have non-positive number for xor %ebx, %ebp, we can avoid jump. 

4) The loop in the winky function will exit when the $rsi mataches one of the strings from the reading file (starting from "purrs"). $rsi here is the second part of my fake password which is after 
   the space excluding the first character. Also, since we want $eax return 26 after passing the winky function, our second part password should matches the "milting" to ensure
   the return value of sum of strcspn will equal to 26.

5) The data being compared is the pointer of the integers and will be dereferenced in the function. The compared values won't shift 4 bit to the right until its value
   is less than or equal 0xf. The order is in the ascending order based on the unsigned value of the compared values.

   e.g if the compared dereferenced value are 0b6, -1
   -1 will be considered larger value.
 
6) The bank's trust model rely on penetration tester like me to find vulnerabilities and trust our ethical compass will lead us to tell them.


