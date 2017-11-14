File openFile(char* name,char* rw) {
  File f=SPIFFS.open(name,rw);
  if (!f) {
    fd.print(timeStamp());
    fd.print(" failed to open ");
    fd.println(name);
  }
  return f;
}
