function decodeUplink(input) {
  var raw = input.bytes[0];
  var raw2 = input.bytes[1];
  var raw3 = input.bytes[2];
  var raw4 = input.bytes[3];
  
  raw = raw - 48;
  raw2 = raw2 - 48;
  raw3 = raw3 - 48;
  raw4 = raw4 - 48;
  
  raw = String(raw);
  raw2 = String(raw2);
  raw3 = String(raw3);
  raw4 = String(raw4);
  
  var pro = raw + raw2 + raw3 + raw4;
  
  return {
    data: {
      data: pro
    },
    warnings: [],
    errors: []
  };
}