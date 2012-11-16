<?php

function heineken_bug_ranking_admin_form( $form, &$form_state ) {

  drupal_add_tabledrag('patterns-table', 'order', 'sibling', 'patterns-weight');

  $header = array(
    'delete'        => t('Delete'),
    'jira' => t('JIRA'),
    'user' => t('User'),
    'type' => t('Type'),
    'project' => t('Project')
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
  $replace_patterns = _heineken_bug_ranking_get_bugs();
  
  foreach ($replace_patterns as $replace_pattern) {
    $rows[] = _heineken_bug_ranking_row_definition($replace_pattern);
  }

  $last_replace_pattern = _heineken_bug_ranking_get_last_row();
  $rows[] = _heineken_bug_ranking_row_definition($last_replace_pattern);
  
  return $rows;
}


function _heineken_bug_ranking_get_bugs() {
  return db_select('heineken_bug_ranking_bug', 'hg')
          ->fields('hg')
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
    'jira' => array(
      '#name' => 'jira' . $replace_pattern->id,
      '#type' => 'textfield',
      '#size' => 30,
      '#value' => isset($replace_pattern->jira) ? $replace_pattern->jira : '',
    ),
    'user' => heineken_bug_ranking_create_setect('user', $replace_pattern),
    'type' => heineken_bug_ranking_create_setect('type', $replace_pattern),
    'project' => heineken_bug_ranking_create_setect('project', $replace_pattern),
  );

  return $row;
}

function heineken_bug_ranking_create_setect($type, $replace_pattern) {

  return array(
      '#name' => $type . $replace_pattern->id,
      '#type' => 'select',
      '#options' => heineken_bug_ranking_create_options($type),
      '#value' => heineken_bug_ranking_value($replace_pattern->id, $type),
    );
}

function heineken_bug_ranking_value($id, $type) {
  $query = db_select('heineken_bug_ranking_bug', 'hb');
  $query->fields('hb');
  $query->condition('id', $id, '=');
  $result = $query->execute();
  $value = '';
  while($record = $result->fetchAssoc()) {
    if(isset($record[$type.'_id'])) {
      $value = $record[$type.'_id'];
    }
  }
  
  return $value;
}

function heineken_bug_ranking_create_options($type) {
  $query = db_select('heineken_bug_ranking_' . $type, $type);
  $query->fields($type);
  $result = $query->execute();
  $options[''] = '- Select one -';
  while($record = $result->fetchAssoc()) {
    $options[$record['id']] = $record['name'];
  }
  return $options;
}

function heineken_bug_ranking_settings_form_submit($form, &$form_state) {
  $replace_patterns = _heineken_bug_ranking_get_bugs();

  foreach ($replace_patterns as $replace_pattern) {
  
    if (isset($form_state['input']['delete' . $replace_pattern->id])) {
      $success = _heineken_bug_ranking_delete_entry_by_id($replace_pattern->id);
      
      if ($success) {
        drupal_set_message(t('The configurations for original bug were successfully deleted.'));
      }
    }
    else {
      _heineken_bug_ranking_update_entry_by_id($replace_pattern->id, $form_state);
    }
    
  }

  if (!empty($form_state['input']['jira0']) && !empty($form_state['input']['user0']) && !empty($form_state['input']['type0']) && !empty($form_state['input']['project0'])) {
    _heineken_bug_ranking_insert_new_entry(_heineken_bug_ranking_get_entry_fields_from_form_state_by_id($form_state));
  }else if (!empty($form_state['input']['jira0']) || !empty($form_state['input']['user0']) || !empty($form_state['input']['type0']) || !empty($form_state['input']['project0'])){
    drupal_set_message(t('Heineken Bug Ranking bug table insertion failed.  All fields Required'), 'error');
  }
}

function _heineken_bug_ranking_insert_new_entry($fields) {
  try {
    $entry = db_insert('heineken_bug_ranking_bug')
      ->fields($fields)
      ->execute();
      
    return $entry;
  }
  catch (Exception $e) {
    drupal_set_message(t('Heineken Bug Ranking bug table insertion failed with the message: :msg', array(':msg' => $e->getMessage())), 'error');
  }
}

function _heineken_bug_ranking_get_entry_fields_from_form_state_by_id(&$form_state, $id = 0) {
  return array(
    'jira' => $form_state['input']['jira'. $id],
    'user_id' => $form_state['input']['user' . $id],
    'project_id' => $form_state['input']['project' . $id],
    'type_id' => $form_state['input']['type' . $id],
  );
}

function _heineken_bug_ranking_delete_entry_by_id($id) {
  $success = db_delete('heineken_bug_ranking_bug')
    ->condition('id', $id)
    ->execute();
  
  return $success;
}

function _heineken_bug_ranking_update_entry_by_id($id, &$form_state) {
  db_update('heineken_bug_ranking_bug')
    ->fields(array(
      'jira' => $form_state['input']['jira'. $id],
      'user_id' => $form_state['input']['user' . $id],
      'project_id' => $form_state['input']['project' . $id],
      'type_id' => $form_state['input']['type' . $id],
    ))
    ->condition('id', $id, '=')
    ->execute();
}