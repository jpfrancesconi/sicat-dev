<?php
/**
 * @file
 * require_login.admin.inc
 */

/**
 * Page callback for require login configurations form.
 */
function require_login_admin_settings() {
  $form = array();
  $form['require_login_deny_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Deny Message'),
    '#description' => t('Access denied message displayed after user login page redirect.'),
    '#default_value' => variable_get('require_login_deny_message', REQUIRE_LOGIN_DEFAULT_MESSAGE),
  );
  $form['require_login_excluded_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude Paths'),
    '#description' => t('Relative paths that shouldn\'t require user login (one per line). Use &lt;front&gt; for front page.'),
    '#default_value' => variable_get('require_login_excluded_paths', ''),
  );
  $form['require_login_auth_path'] = array(
    '#type' => 'textfield',
    '#title' => t('User Login Path'),
    '#description' => t('Relative path to user login page (if different from /user/login). Supports URL queries and fragments.') .'<br />'.
      t('Example: /login?destination=node/1'),
    '#default_value' => variable_get('require_login_auth_path', ''),
  );
  return system_settings_form($form);
}

/**
 * Validate handler for require login configurations form.
 */
function require_login_admin_settings_validate($form, &$form_state) {
  $absolutes = array();

  // Confirm exclusion paths are relative. Also trims and confirms presence
  // of leading forward slash on paths. We don't check if page actually exists.
  $exclude_paths = explode(PHP_EOL, $form_state['values']['require_login_excluded_paths']);
  foreach ($exclude_paths as $key => $exclude_path) {
    $exclude_path = trim($exclude_path);
    $url = parse_url($exclude_path);

    // Detect invalid absolute domain in path.
    if (isset($url['scheme']) || isset($url['host']) || preg_match('/^www./i', $url['path'])) {
      $absolutes[] = trim($exclude_path);
    }

    // Confirm leading forward slash presence.
    else if (substr($exclude_path, 0, 1) != '/') {
      $exclude_paths[$key] = '/'. $exclude_path;
    }

    // Trim unnecessary whitespace from ends.
    else {
      $exclude_paths[$key] = $exclude_path;
    }
  }
  if ($absolutes) {
    form_set_error('require_login_excluded_paths', t('All exclusion paths must be relative. Invalid paths:<br />!paths', array(
      '!paths' => implode('<br />', $absolutes),
    )));
  }
  $form_state['values']['require_login_excluded_paths'] = implode(PHP_EOL, $exclude_paths);

  // Confirm user login path is relative. Also trims and confirms presence
  // of leading forward slash. We don't check if page actually exists.
  if (!empty($form_state['values']['require_login_auth_path'])) {
    $auth_path = trim($form_state['values']['require_login_auth_path']);
    $url = parse_url($auth_path);

    // Detect invalid absolute domain in path.
    if (isset($url['scheme']) || isset($url['host']) || preg_match('/^www./i', $url['path'])) {
      form_set_error('require_login_auth_path', t('User login path must be relative.'));
    }

    // Confirm leading forward slash presence.
    else if (substr($auth_path, 0, 1) != '/') {
      $form_state['values']['require_login_auth_path'] = '/'. $auth_path;
    }

    // Trim unnecessary whitespace from ends.
    else {
      $form_state['values']['require_login_auth_path'] = $auth_path;
    }
  }
}
