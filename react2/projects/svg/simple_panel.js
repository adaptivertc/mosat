
function simple_panel_t(panel_name, on_color, off_color)
{
  console.log("panel_name " + panel_name);
  this.on_color = on_color;
  this.off_color = off_color;
  console.log("on_color " + this.on_color);
  console.log("off_color " + this.off_color);
  this.panel_obj=document.getElementById(panel_name);
  console.log("panel_obj: " + this.panel_obj);
}

function simple_panel_init_f(val)
{
}
simple_panel_t.prototype.init=simple_panel_init_f;

function simple_panel_update_f(pv)
{
  console.log("pv = " + pv + ", obj = " + this.panel_obj);
  if (pv > 50)
    this.panel_obj.setAttribute("fill", this.on_color);
  else 
    this.panel_obj.setAttribute("fill", this.off_color);
}
simple_panel_t.prototype.update=simple_panel_update_f;

