<?php

function heineken_bug_ranking_admin_form( $form, &$form_state ) {

  drupal_add_tabledrag('patterns-table', 'order', 'sibling', 'patterns-weight');

  $header = array(
    'delete'        => t('Delete'),
    'pattern' => t('Original Text')
  );
  
  $form['patterns_table'] = array(
    '#tree' => TRUE,
    '#header' => $header,
    '#rows' => _heineken_bug_ranking_get_table_rows(),
    '#theme' => 'table',
    '#attributes' => array('id' => 'patterns-table'),
    '#empty'  => t('No content available.'),
  );
  
  $form['#submit'][] = 'heineken_bug_ranking_settings_form_submit';
  
  return system_settings_form($form);

}

function _heineken_bug_ranking_get_table_rows() {  
  module_load_include('inc', 'heineken_bug_ranking', 'inc/heineken_bug_ranking.common'); 
  $rows = array();
  $replace_patterns = _heineken_bug_ranking_get_types();
  
  foreach ($replace_patterns as $replace_pattern) {
    $rows[] = _heineken_bug_ranking_row_definition($replace_pattern);
  }

  $last_replace_pattern = _heineken_bug_ranking_get_last_row();
  $rows[] = _heineken_bug_ranking_row_definition($last_replace_pattern);
  
  return $rows;
}


function _heineken_bug_ranking_get_types() {
  return db_select('heineken_bug_ranking_type', 'ht')
          ->fields('ht')
          ->execute();
}

function _heineken_bug_ranking_get_filled_row($replace_pattern) {
  module_load_include('inc', 'heineken_bug_ranking', 'inc/heineken_bug_ranking.common'); 
  if (!isset($replace_pattern->id)) {
    $replace_pattern = _heineken_bug_ranking_get_last_row();
  }

  $row = array(
    'delete' => array(
      '#name' => 'delete' . $replace_pattern->id,
      '#type' => 'checkbox',
      '#checked' => FALSE,
    ),
    'type' => array(
      '#name' => 'type' . $replace_pattern->id,
      '#type' => 'textfield',
      '#size' => 30,
      '#value' => isset($replace_pattern->name) ? $replace_pattern->name : '',
    ),
  );

  return $row;
}

function heineken_bug_ranking_settings_form_submit($form, &$form_state) {
  $replace_patterns = _heineken_bug_ranking_get_types();

  foreach ($replace_patterns as $replace_pattern) {
  
    if (isset($form_state['input']['delete' . $replace_pattern->id])) {
      $success = _heineken_bug_ranking_delete_entry_by_id($replace_pattern->id);
      
      if ($success) {
        drupal_set_message(t('The configurations for original text: :type type were successfully deleted.', array(':type' => $replace_pattern->name)));
      }
    }
    else {
      _heineken_bug_ranking_update_entry_by_id($replace_pattern->id, $form_state);
    }
    
  }

  if (!empty($form_state['input']['type0'])) {
    _heineken_bug_ranking_insert_new_entry(_heineken_bug_ranking_get_entry_fields_from_form_state_by_id($form_state));
  }
}

function _heineken_bug_ranking_insert_new_entry($fields) {
  try {
    $entry = db_insert('heineken_bug_ranking_type')
      ->fields($fields)
      ->execute();
      
    return $entry;
  }
  catch (Exception $e) {
    drupal_set_message(t('Heineken Bug Ranking type table insertion failed with the message: :msg', array(':msg' => $e->getMessage())), 'error');
  }
}

function _heineken_bug_ranking_get_entry_fields_from_form_state_by_id(&$form_state, $id = 0) {
  return array(
    'name' => $form_state['input']['type' . $id],
  );
}

function _heineken_bug_ranking_delete_entry_by_id($id) {
  $success = db_delete('heineken_bug_ranking_type')
    ->condition('id', $id)
    ->execute();
  
  return $success;
}

function _heineken_bug_ranking_update_entry_by_id($id, &$form_state) {
  db_update('heineken_bug_ranking_type')
    ->fields(array(
      'name' => $form_state['input']['type' . $id],
    ))
    ->condition('id', $id, '=')
    ->execute();
}