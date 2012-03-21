var sim;

    function pump_1_timeout()
    {
      sim.toggle_pump1();
    };

    function pump_2_timeout()
    {
      sim.toggle_pump2();
    };

    function pump_3_timeout()
    {
      sim.toggle_pump3();
    };

function sim_object_t()
{
  this.lo_level=90;
  this.hi_level=260;
  this.hi_hi_level=300;
  this.rate=0;
  this.up=true;

  this.pv =
  {
    "LEVEL":100,
    "PUMP1_AMP":0,
    "PUMP2_AMP":0,
    "PUMP3_AMP":0,
    "PUMP1_ON":true,
    "PUMP2_ON":true,
    "PUMP3_ON":true,
    "HI_HI_LEVEL":false,
    "HI_LEVEL":false,
    "LO_LEVEL":true
  }

  this.cfg =
  {
    "LEVEL":{"tag":"LEVEL","description":"NIVEL","eu":"cm","driver":0,"card":0,"channel":2,"eu_lo":0.000000,"eu_hi":500.000000,"raw_lo":32000.000000,"raw_hi":6400.000000,"decimal_places":0,"zero_cutoff":-125.000000,"lo_alarm":0.000000,"lo_caution":0.000000,"hi_caution":370.000000,"hi_alarm":400.000000,"deadband":0.000000,"lo_alarm_enable":false,"lo_caution_enable":false,"hi_caution_enable":true,"hi_alarm_enable":true, "scale_lo":0,"scale_hi":500},
    "PUMP1_AMP":{"tag":"PUMP1_AMP","description":"BOMBA 1","eu":"amps","driver":0,"card":0,"channel":5,"eu_lo":0.000000,"eu_hi":50.000000,"raw_lo":6400.000000,"raw_hi":32000.000000,"decimal_places":1,"zero_cutoff":-12.500000,"lo_alarm":0.000000,"lo_caution":0.000000,"hi_caution":30.000000,"hi_alarm":35.000000,"deadband":0.000000,"lo_alarm_enable":false,"lo_caution_enable":false,"hi_caution_enable":true,"hi_alarm_enable":true,"scale_lo":0,"scale_hi":50},
    "PUMP2_AMP":{"tag":"PUMP2_AMP","description":"BOMBA 2","eu":"amps","driver":0,"card":0,"channel":4,"eu_lo":0.000000,"eu_hi":50.000000,"raw_lo":6400.000000,"raw_hi":32000.000000,"decimal_places":1,"zero_cutoff":-12.500000,"lo_alarm":0.000000,"lo_caution":0.000000,"hi_caution":30.000000,"hi_alarm":35.000000,"deadband":0.000000,"lo_alarm_enable":false,"lo_caution_enable":false,"hi_caution_enable":true,"hi_alarm_enable":true,"scale_lo":0,"scale_hi":50},
    "PUMP3_AMP":{"tag":"PUMP3_AMP","description":"BOMBA 3","eu":"amps","driver":0,"card":0,"channel":3,"eu_lo":0.000000,"eu_hi":50.000000,"raw_lo":6400.000000,"raw_hi":32000.000000,"decimal_places":1,"zero_cutoff":-12.500000,"lo_alarm":0.000000,"lo_caution":0.000000,"hi_caution":30.000000,"hi_alarm":35.000000,"deadband":0.000000,"lo_alarm_enable":false,"lo_caution_enable":false,"hi_caution_enable":true,"hi_alarm_enable":true,"scale_lo":0,"scale_hi":50},
    "PUMP1_ON": {"tag":"PUMP1_ON","description":"Bomba 1 ON","driver":0,"card":0,"channel":8, "lo_desc":"Off","hi_desc":"ON", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
    "PUMP2_ON":{"tag":"PUMP2_ON","description":"Bomba 1 ON","driver":0,"card":0,"channel":9, "lo_desc":"Off","hi_desc":"ON", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
    "PUMP3_ON":{"tag":"PUMP3_ON","description":"Bomba 1 ON","driver":0,"card":0,"channel":10, "lo_desc":"Off","hi_desc":"ON", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
    "HI_HI_LEVEL":{"tag":"HI_HI_LEVEL","description":"Nivel Muy Alto","driver":0,"card":1,"channel":2, "lo_desc":"BELOW","hi_desc":"ABOVE", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
    "HI_LEVEL":{"tag":"HI_LEVEL","description":"Nivel Alto","driver":0,"card":1,"channel":1, "lo_desc":"BELOW","hi_desc":"ABOVE", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
    "LO_LEVEL":{"tag":"LO_LEVEL","description":"Nivel Bajo","driver":0,"card":1,"channel":0, "lo_desc":"BELOW","hi_desc":"ABOVE", "alarm_state":"NONE","shutdown_state":"NONE","invert_pv":false},
  }
} 

function sim_object_init_f(val)
{

}
sim_object_t.prototype.init=sim_object_init_f;

function toggle_pump2_f(val)
{
  this.pv.PUMP2_ON = !this.pv.PUMP2_ON
  console.log("Toggle pump 2 to:", this.pv.PUMP2_ON);
}
sim_object_t.prototype.toggle_pump2=toggle_pump2_f;

function toggle_pump3_f(val)
{
  this.pv.PUMP3_ON = !this.pv.PUMP3_ON
  console.log("Toggle pump 3 to:", this.pv.PUMP3_ON);
}
sim_object_t.prototype.toggle_pump3=toggle_pump3_f;

function toggle_pump1_f(val)
{
  this.pv.PUMP1_ON = !this.pv.PUMP1_ON
  console.log("Toggle pump 1 to:", this.pv.PUMP1_ON);
}

sim_object_t.prototype.toggle_pump1=toggle_pump1_f;

function sim_object_get_cfg_f(tag)
{
  console.log("getting config for: " + tag);
  var the_val = this.cfg[tag.toUpperCase()];
  console.log("cfg value: " + the_val);
  return the_val;
}
sim_object_t.prototype.get_cfg=sim_object_get_cfg_f;

function sim_object_get_pv_f(tag)
{
  //console.log("getting PV for: " + tag);
  var the_val = this.pv[tag.toUpperCase()];
  //console.log("PV: " + the_val);
  return the_val;
}
sim_object_t.prototype.get_pv=sim_object_get_pv_f;

    function sim_object_update_f()
    {
      if (this.up)
      {
        if (this.pv.LEVEL > (this.lo_level+2)) this.pv.LO_LEVEL=true;
        if (this.pv.LEVEL > (this.hi_level+1))
        { 
          this.up=false;
          this.pv.HI_LEVEL=true;
          setTimeout("pump_1_timeout()", 1000);
          setTimeout("pump_2_timeout()", 4000);
          setTimeout("pump_3_timeout()", 7000);
        }
        if (this.pv.LEVEL > (this.hi_hi_level+2)) this.pv.HI_HI_LEVEL=true;
      }
      else
      {
        if (this.pv.LEVEL < (this.lo_level-1)) 
        {
          this.up=true;
          this.pv.LO_LEVEL=false;
          setTimeout("pump_3_timeout()", 1000);
          setTimeout("pump_2_timeout()", 4000);
          setTimeout("pump_1_timeout()", 7000);
        }
        if (this.pv.LEVEL < (this.hi_level-3)) this.pv.HI_LEVEL=false;
        if (this.pv.LEVEL < (this.hi_hi_level-3)) this.pv.HI_HI_LEVEL=false;
      }
      this.rate=10.0;
      if (!this.pv.PUMP1_ON) 
      {
        this.rate -= 6.0;
        this.pv.PUMP1_AMP= (27 +((Math.random()-0.5)*0.6));
      }
      else
      {
        this.pv.PUMP1_AMP= (0 +((Math.random()-0.5)*0.6));
      }

      if (!this.pv.PUMP2_ON) 
      {
        this.rate -= 6.0;
        this.pv.PUMP2_AMP= (27 +((Math.random()-0.5)*0.6));
      }
      else
      {
        this.pv.PUMP2_AMP= (0 +((Math.random()-0.5)*0.6));
      }

      if (!this.pv.PUMP3_ON)
      {
        this.rate -= 6.0;
        this.pv.PUMP3_AMP= (27 +((Math.random()-0.5)*0.6));
      }
      else
      {
        this.pv.PUMP3_AMP= (0 +((Math.random()-0.5)*0.6));
      }
      this.pv.LEVEL+=this.rate*0.1;
      this.pv.LEVEL= ((this.pv.LEVEL+((Math.random()-0.5)*0.8)));
    }
    sim_object_t.prototype.update=sim_object_update_f;

sim = new sim_object_t;

