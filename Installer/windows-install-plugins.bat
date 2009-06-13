rmdir "..\bin\plugins" /s /q
mkdir "..\bin\plugins"
FOR /R "..\resources" %%v IN (*.js) DO copy "%%v" "..\bin\plugins"