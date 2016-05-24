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
    $handsontable_row = reset($handsontable_rows);

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
    $has_data = false;
    foreach (element_children($element) as $key) {
      $values[$key] = [];
      foreach (element_children($element[$key]) as $subKey) {
        if (isset($element[$key][$subKey]['#default_value'])) {

          if ( !empty($element[$key][$subKey]['#default_value']) ) {
            $has_data = true;
          }

          // TODO: We probably have to handle order of columns here
          $values[$key][] = $element[$key][$subKey]['#default_value'];
        }
        else {
          $values[$key][] = NULL;
        }
      }
    }

    // TODO: I don't like this $has_data check, can we do this more pretty.
    // It is required to let handsontable know we have an empty table, and to make the number
    // of rows work in an empty table
    if ( !$has_data ) {
      $values = null;
    }

    // Discover columns, they are the childs of the first handsontable_row element
    $children = array_intersect_key($handsontable_row, array_flip(element_children($handsontable_row)));

    $colHeaders = [];
    $columns = [];
    foreach ($children as $name => $child) {
      $colHeaders[] = isset($child['#title']) ? $child['#title'] : 'no-title';
      $columns[] = [
        'name' => $name,
        'type' => isset($child['#autocomplete_path']) ? 'autocomplete' : 'text',
        'path' => isset($child['#autocomplete_path']) ? $child['#autocomplete_path'] : null
      ];
    }

    // Pass settings
    $element['#attached']['js'][] = [
      'data' => [
        'handsontable' => [
          'names' => [$element['#name']],
          'data' => [$values],
          'colHeaders' => [$colHeaders],
          'columns' => [$columns],
          'startRows' => [isset($element['#rows']) && is_numeric($element['#rows']) ? $element['#rows'] : 0]
        ]
      ],
      'type' => 'setting',
    ];

    // Add AJAX callback
    $element['addRow'] = self::createChangeCallback($element, $form);

    // This hidden input will contain the number of handsontable rows
    // This is used in the AJAX callback
    $element['rows'] = [
      '#type' => 'hidden',
      '#prefix' => '<div id="' . $element['#id'] . '-rows">',
      '#suffix' => '</div>'
    ];

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
    // TODO: This doesn't look entirely correct to build the "edit-" yourself. 
    // It seemed to be required though to have the right triggering element in
    // the callback
    $add['#id'] =  $add['#name'] =  'edit-' . $element['#name'] . '-callback';
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