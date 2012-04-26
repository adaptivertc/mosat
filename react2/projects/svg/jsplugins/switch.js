
function switch_t(group_name, on_name, off_name, on_color, off_color, cx, cy, hstr)
{
  console.log("group_name " + group_name);
  this.on_color = on_color;
  this.off_color = off_color;
  console.log("on_color " + this.on_color);
  console.log("off_color " + this.off_color);
  this.group_obj=document.getElementById(group_name);
  this.on_obj=document.getElementById(on_name);
  this.off_obj=document.getElementById(off_name);
  this.interval_handler_str=hstr;
  this.state=false;
  this.in_limbo=true; // Waiting for first update
  this.interval_id=null;
  this.cx = cx;
  this.cy = cy;
  console.log("group_obj: " + this.group_obj);
}

function switch_init_f(val)
{
  this.lo_desc = val.lo_desc;
  this.hi_desc = val.hi_desc;
}
switch_t.prototype.init=switch_init_f;

function switch_update_f(pv)
{
  return;
  //console.log("pv = " + pv + ", obj = " + this.group_obj);
  if (pv)
  {
    this.on_obj.setAttribute("fill", this.on_color);
    this.off_obj.setAttribute("fill", gray);
  }
  else 
  {
    this.on_obj.setAttribute("fill", gray);
    this.off_obj.setAttribute("fill", this.off_color);
  }
}
switch_t.prototype.update=switch_update_f;

function switch_interval_handler_f()
{
  if (this.turning_on)
  {
    this.position += 5;
    console.log("OBJECT!!! Position: " + this.position);
    if (this.position >= 45)
    {
      this.position = 45;
      clearInterval(this.interval_id);
    }
  }
  else
  {
    this.position -= 5;
    if (this.position <= -45)
    {
      this.position = -45;
      clearInterval(this.interval_id);
    }
  }
  var str = "rotate(" + this.position + " " + this.cx + "," + this.cy + ")";
  console.log("transform: " + str);
  this.group_obj.setAttribute("transform", str);
}
switch_t.prototype.interval_handler=switch_interval_handler_f;

function switch_off_clicked_f()
{
  if (!this.state) return;
  console.log("OBJECT!!!! Clicked on r1");
  this.state=false;
  this.turning_on = false;
  this.position = 45;
  this.off_obj.setAttribute("fill", this.off_color);
  this.on_obj.setAttribute("fill", "gray");
  this.interval_id = setInterval(this.interval_handler_str, 20);
}
switch_t.prototype.off_clicked=switch_off_clicked_f;

function switch_on_clicked_f()
{
  if (this.state) return;
  console.log("OBJECT!!!! Clicked on r2");
  this.state=true;
  this.turning_on = true;
  this.position = -45;
  this.off_obj.setAttribute("fill", "gray");
  this.on_obj.setAttribute("fill", this.on_color);
  this.interval_id = setInterval(this.interval_handler_str, 20);
}
switch_t.prototype.on_clicked=switch_on_clicked_f;
