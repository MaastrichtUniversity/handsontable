<?php

/**
 * @file
 * Defines callbacks and resources needed for the 'handsontable' form element.
 */
class Handsontable {

  /**
   * Loads the required resources for displaying the Handsontable element.
   * @param $form_state
   */
  public static function addRequiredResources(&$form_state) {
    drupal_add_css(drupal_get_path('module', 'handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.css', array(
      'group' => CSS_DEFAULT,
      'every_page' => TRUE
    ));
    drupal_add_js(drupal_get_path('module', 'handsontable') . '/bower_components/handsontable/dist/handsontable.full.min.js', array(
      'group' => CSS_DEFAULT,
      'every_page' => TRUE
    ));
  }

  /**
   * The '#process' callback for the handsontable_row form element.
   *
   * @param array $element
   *   The handsontable_row form element.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $form
   *   The complete Drupal form definition.
   *
   * @return array
   *   The handsontable_row form element.
   */
  public static function process(array $element, array &$form_state, array $form = NULL) {
    self::addRequiredResources($form_state);

    $children = array_intersect_key($element, array_flip(element_children($element)));
    $handsontable_rows = array_filter($children, ['self', 'FilterChildren']);

    if ( count($handsontable_rows) === 0 ) {
      drupal_set_message(t('Attempted to create handsontable %name without defining a handsontable_row.', array('%name' => $element['#name'])), 'error');
      return $element;
    }

    // Config JS
    $element['#attached']['js'] = [
      drupal_get_path('module', 'handsontable') . '/js/handsontable_field.config.js'
    ];

    // Determine already set values
    $values = [];
    foreach (element_children($element) as $key) {
      $values[$key] = [];
      foreach (element_children($element[$key]) as $subKey) {
        if (isset($element[$key][$subKey]['#default_value'])) {
          $values[$key][$subKey] = $element[$key][$subKey]['#default_value'];
        }
        else {
          $values[$key][$subKey] = NULL;
        }
      }
    }

    // Discover columns
    $children = array_intersect_key($handsontable_rows[0], array_flip(element_children($handsontable_rows[0])));

    $colHeaders = [];
    $columns = [];
    foreach ($children as $name => $child) {
      $colHeaders[] = $child['#title'];
      $columns[] = $name;
    }

    // Pass settings
    $element['#attached']['js'][] = [
      'data' => [
        'handsontable' => [
          'names' => [$element['#name']],
          'data' => [json_encode($values)],
          'colHeaders' => [$colHeaders],
          'columns' => [$columns],
          'startRows' => [$element['#rows']]
        ]
      ],
      'type' => 'setting',
    ];

    // Add AJAX callback
    $element['addRow'] = self::createChangeCallback($element, $form);

    return $element;
  }

  /**
   * Creates an invisible AJAX callback button that allows change number of handsontable rows.
   *
   * @param array $element
   *   The handsontable form element.
   * @param array $complete_form
   *   The completed form.
   *
   * @return \FormElement The "add" button.
   *
   */
  protected static function createChangeCallback(array &$element, array &$complete_form) {
    $add['#type'] = 'button';
    $add['#id'] = $add['#name'] = $element['#id'] . '-callback';
    $add['#attributes'] = array('style' => 'display:none');
    $add['#ajax'] = array(
      'params' => array(
        'target' => $element['#hash']
      ),
      'callback' => 'xml_form_elements_ajax_callback',
      'method' => 'settings'
    );
    return $add;
  }

  /**
   * Checks if a child element has the 'handsontable_row' #type.
   *
   * @param array $child
   *   The child to determine.
   *
   * @return bool
   *   TRUE if it is a tabpanel, FALSE otherwise.
   */
  public static function FilterChildren(array $child) {
    $ret = ($child['#type'] == 'handsontable_row') ? TRUE : FALSE;
    return $ret;
  }
}

/**
 * Implements theme_hook().
 *
 * @param array $variables
 *   An array of theming variables.
 * @return string
 */
function theme_handsontable($variables) {
  $element = $variables['element'];

  unset($element['#prefix']);
  unset($element['#suffix']);

  $output = '<div class="handsontable-container">';

  $output .= '<div id="' . $element['#id'] . '"></div>';
  $output .= '<div id="' . $element['#id'] . '-table" class="handsontable"></div>';

  $output .= $element['#children'];
  $output .= '</div>';

  return $output;

}