

function ScaleInfo()
{
  this.max_val=0.0;
  this.max_major=0.0;
  this.major_inc=0.0;
  this.minor_inc=0.0;
  this.n_major=0;
  this.n_minor=0;
  this.n_dec=0;
}

function f_do_print(max)
{
  console.log("max_val = " + this.max_val);
  console.log("max_major = " + this.max_major);
  console.log("major_inc = " + this.major_inc);
  console.log("minor_inc = " + this.minor_inc);
  console.log("n_major = " + this.n_major);
  console.log("n_minor = " + this.n_minor);
  console.log("n_dec = " + this.n_dec);
}

ScaleInfo.prototype.print=f_do_print;

function log10(a_val)
{
  return Math.log(a_val) / 2.302585;
}


function f_do_calcs(max)
{
  var the_log = log10(max); 
  var the_floor = Math.floor(the_log);
  var z99 = max / Math.pow(10.0, (the_floor - 1)); 
  var the_scale=0.0;
  var top_major=0.0;

  console.log("log = ", the_log);
  console.log("floor = " + the_floor);
  console.log("z99 = " + z99);

  if (z99 >= 97.0) 
  {
    the_scale = 100;
    top_major = 100;
    this.n_major = 5;
    this.n_minor = 4;
  } 
  else if (z99 >= 80.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 80;
    this.n_major = 4;
    this.n_minor = 4;
  }
  else if (z99 >= 60.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 60;
    this.n_major = 3;
    this.n_minor = 4;
  }
  else if (z99 >= 50.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 50;
    this.n_major = 5;
    this.n_minor = 4;
  }
  else if (z99 >= 40.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 40;
    this.n_major = 4;
    this.n_minor = 4;
  }
  else if (z99 >= 35.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 35;
    this.n_major = 7;
    this.n_minor = 2;
  }
  else if (z99 >= 30.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 30;
    this.n_major = 3;
    this.n_minor = 4;
  }
  else if (z99 >= 25.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 25;
    this.n_major = 5;
    this.n_minor = 2;
  }
  else if (z99 >= 20.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 20;
    this.n_major = 4;
    this.n_minor = 5;
  }
  else if (z99 >= 18.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 18;
    this.n_major = 6;
    this.n_minor = 3;
  }
  else if (z99 >= 15.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 15;
    this.n_major = 5;
    this.n_minor = 3;
  }
  else if (z99 >= 14.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 14;
    this.n_major = 7;
    this.n_minor = 2;
  }
  else if (z99 >= 12.0) 
  {
    the_scale = Math.ceil(z99);
    top_major = 12;
    this.n_major = 6;
    this.n_minor = 2;
  }
  else
  {
    the_scale = Math.ceil(z99);
    top_major = 10;
    this.n_major = 5;
    this.n_minor = 2;
  }
  console.log("top = " + the_scale);
  console.log("top major = " + top_major);
  console.log("N major = " + this.n_major);
  console.log("N minor = " + this.n_minor);
  this.n_dec = 0; 
  if (the_floor < 1)
  {
    this.n_dec = (-the_floor) + 1;
  }

  this.major_inc = ((top_major) /(this.n_major)) *  Math.pow(10.0, (the_floor - 1));
  this.minor_inc = ((top_major) / (this.n_major * this.n_minor)) *  Math.pow(10.0, (the_floor - 1));
  this.max_val = (the_scale) *  Math.pow(10.0, (the_floor - 1));
  this.max_major = (top_major) *  Math.pow(10.0, (the_floor - 1));
}

ScaleInfo.prototype.calc=f_do_calcs;
