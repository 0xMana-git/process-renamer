# driver to rename process     
# how 2 use:    
set pid and ur process name then compile, use [magic](https://github.com/hfiref0x/KDU) to load   
it only changes stuff in the eprocess entry so process name should be "visibly" unaffected, you should be able to see changes when you use Get-Process on powershell  
if you want the changes to be reflected in other stuff as well u can probably modify the peb of the process(remember to use kestackattachprocess)  

<sub><sup><sub><sup><sub><sup><sub><sup><sub><sup><sub><sup><sub><sup><sub><sup>also i know u dont give a shit about this but i wasted 6 hours of my life wrestling with ntoskrnl on ida cuz i thought my shit didnt work </sup></sub></sup></sub></sup></sub></sup></sub></sup></sub></sup></sub></sup></sub></sup></sub>