
function bar_t(bar_name, text_name, y1, y2, x2, stroke_width)
{
  console.log("bar_name " + bar_name);
  console.log("text_name " + text_name);
  this.length=y2-y1;
  console.log("length " + this.length);
  this.max=200.0;
  this.bar_obj=document.getElementById(bar_name);
  this.text_obj=document.getElementById(text_name);
  this.eu_label="not set";
  this.y1=y1;
  this.y2=y2;
  this.x2=x2;
  this.stroke_width = stroke_width;
  console.log("bar_obj: " + this.bar_obj);
  console.log("text_obj: " + this.text_obj);
}

function bar_init_f(val)
{
  this.max=val.scale_hi;
  this.eu_label=val.eu;
  this.decimal_places = val.decimal_places;
  console.log("max: " + this.max);
  console.log("eu_label: " + this.eu_label);
      var sinfo = new ScaleInfo();
      var tdata = new TicData();
      var shape;
      sinfo.print();
      sinfo.calc(this.max,this.y2,this.y1);
      sinfo.print();
      sinfo.reset_tic();
      while(sinfo.next_tic(tdata))
      {
        var mystr;
        if (tdata.is_major)
        {
          console.log("======== " + tdata.eu_val + ", " + tdata.screen_val);
          shape = document.createElementNS(svgNS, "line");
          shape.setAttribute("x1", this.x2);
          shape.setAttribute("y1", tdata.screen_val);
          shape.setAttribute("x2", this.x2+(this.length*0.02));
          shape.setAttribute("y2", tdata.screen_val);
          shape.setAttribute("style", "stroke:black;stroke-width:" + this.stroke_width);
          document.documentElement.appendChild(shape);

          var text_height = this.length * 0.06; 
          shape = document.createElementNS(svgNS, "text");
          shape.setAttribute("x", this.x2 + (this.length * 0.03));
          shape.setAttribute("y", tdata.screen_val + (text_height * 0.33));
          shape.setAttribute("font-size", text_height);
          //shape.setAttribute("baseline-shift", "-33%");
          shape.setAttribute("text-anchor", "start");
          shape.textContent=tdata.eu_val.toFixed(this.decimal_places);
          document.documentElement.appendChild(shape);

        }
        else
        {
          console.log("- " + tdata.eu_val + ", " + tdata.screen_val);
          shape = document.createElementNS(svgNS, "line");
          shape.setAttribute("x1", this.x2);
          shape.setAttribute("y1", tdata.screen_val);
          shape.setAttribute("x2", this.x2+(this.length * 0.015));
          shape.setAttribute("y2", tdata.screen_val);
          shape.setAttribute("style", "stroke:black;stroke-width:" + (this.stroke_width * 0.5));
          document.documentElement.appendChild(shape);
        }
      }

}
bar_t.prototype.init=bar_init_f;

function bar_update_f(pv)
{
  //console.log("pv: " + pv + ", bar_obj = " + this.bar_obj);
  this.text_obj.textContent = pv.toFixed(this.decimal_places) + " " + this.eu_label;
  if (pv < 0.0) {pv = 1.0;}
  //console.log("max: " + this.max);
  //console.log("length: " + this.length);
  this.bar_obj.setAttribute("height", pv * (this.length / this.max));
}
bar_t.prototype.update=bar_update_f;

