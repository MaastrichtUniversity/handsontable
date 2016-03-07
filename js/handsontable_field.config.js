Drupal.behaviors.handsontable = {
    attach: function (context, settings) {

        function updateTable(table, container) {
            var datadump = jQuery(table).handsontable('getData'),
                dataString = JSON.stringify(datadump);
            jQuery(container).val(dataString);
        }

        // Initiate handsontable and add constructor options.
        jQuery.each(Drupal.settings.handsontable.ids, function(i, val) {
            var container = '#' + val,
                table = container + '-table';

            jQuery(table).handsontable({
                data: JSON.parse(Drupal.settings.handsontable.data[i]),
                minSpareRows: 1,
                startCols: Drupal.settings.handsontable.colHeaders[i].length,
                startRows: Drupal.settings.handsontable.startRows[i],
                colHeaders: Drupal.settings.handsontable.colHeaders[i],
                columnSorting: true,
                contextMenu: ['remove_row', 'undo'],
                stretchH: 'all',
                allowInsertColumn: false,
                allowRemoveColumn: false,
                outsideClickDeselects: false,
                persistentState : true,
                beforeChange: function () {
                    updateTable(table, container);
                },
                afterChange: function () {
                    updateTable(table, container);
                },
                afterRemoveRow: function() {
                    updateTable(table, container);
                },
                afterRemoveCol: function() {
                    updateTable(table, container);
                }
            });
        });

        var actions = {
            addRow: function (event, ht) {
                ht.alter('insert_row');
            }
        };

        jQuery(document.body).delegate('div.handsontable-container a[data-action]','click', function (event) {
            var action = jQuery(this).data('action'),
                name =  jQuery(this).parents('div.handsontable-container').find('div.handsontable').attr('id'),
                ht = jQuery('#' + name).handsontable('getInstance');

            event.preventDefault();

            // If there's an action with the given name, call it
            if( typeof actions[action] === 'function' ) {
                actions[action].call(this, event, ht, name);
            }
        });
    }
};
