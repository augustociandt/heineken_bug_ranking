<?php
function heineken_bug_ranking_admin_form( $form, &$form_state ) {
  /*$form = array();
  $form['add_user'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Add users',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE
  );

  $form['add_user']['name'] = array(
    '#type'        => 'textfield',
    '#title'       => 'User',
    '#description' => 'Insert user name'
  );

  $form['user_update'] = array(
    '#type'        => 'fieldset',
    '#title'       => 'Users update',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE
  );*/

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

}