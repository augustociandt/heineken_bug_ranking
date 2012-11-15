<?php


function _heineken_bug_ranking_row_definition($replace_pattern) {

  $row = _heineken_bug_ranking_get_filled_row($replace_pattern);
  $row = _heineken_bug_ranking_get_rendered_row_fields($row);

  $row_definition = array(
    'data' => $row,
    'class' => array('draggable'),
  );
  
  return $row_definition;
}

function _heineken_bug_ranking_get_last_row() {
  $replace_pattern = new stdClass();
  $replace_pattern->id = 0;
  
  return $replace_pattern;
}

function _heineken_bug_ranking_get_rendered_row_fields($row) {
  foreach($row as $field => $row_data) {
    drupal_render($row_data);
    $row[$field] = $row_data['#children'];
  }
  
  return $row;
}