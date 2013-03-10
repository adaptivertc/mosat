function transform_mouse_xy(evt, svg_obj)
{
  var point = document.documentElement.createSVGPoint();
  point.x = evt.clientX;
  point.y = evt.clientY;
  var ctm = svg_obj.getScreenCTM();
  point = point.matrixTransform(ctm.inverse());
  return point;
}
