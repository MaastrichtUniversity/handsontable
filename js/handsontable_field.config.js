Drupal.behaviors.handsontable = {
    attach: function (context, settings) {

        // Adds all table rows and columns to input fields, so the Drupal form processor can handle it
        function updateTable(name, table, container, columns) {
            var data = jQuery(table).handsontable('getData');

            jQuery(container).empty();

            jQuery(data).each(function (row) {
                jQuery(columns).each(function (column, columnName) {
                    if (data[row][column] != null && data[row][column] != "") {
                        jQuery(container).append(
                            jQuery("<input type='hidden' name='" + name + "[" + row + "][" + columnName + "]'>").val(data[row][column])
                        )
                    }
                });
            });
        }

        // Trigger AJAX callback
        function triggerCallback(container) {
            jQuery(container + '-callback')
                .trigger("mousedown")
        }

        function columnAutocomplete(query, process, path) {
            jQuery.ajax({
                url: settings.basePath + path + "/" + query,
                dataType: 'json',
                success: function (response) {
                    // Drupal sends autocomplete in array of objects, handsontable expects array
                    var array = [];
                    jQuery.each(response, function(value, index) {
                        array.push(value);
                    });
                    process(array);
                }
            });
        }

        function buildColumns(columns) {
            var r = [];

            jQuery(columns).each(function(i, column) {
                s = {
                    'type': column.type
                };

                if (s.type == 'autocomplete') {
                    s.source = function(query, process) {
                        columnAutocomplete(query, process, column.path)
                    };

                    s.strict = true;
                }

                r.push(s);
            });

            return r;
        }

        // Initiate handsontable and add constructor options.
        jQuery.each(settings.handsontable.names, function (i, name) {
            var container = '#edit-' + name,
                table = container + '-table';

            jQuery(table, context).once('processed', function () {
                jQuery(table).handsontable({
                    data: settings.handsontable.data[i],
                    minSpareRows: 1,
                    startCols: settings.handsontable.colHeaders[i].length,
                    startRows: settings.handsontable.startRows[i],
                    colHeaders: settings.handsontable.colHeaders[i],
                    columnSorting: true,
                    contextMenu: ['remove_row', 'undo'],
                    stretchH: 'all',
                    allowInsertColumn: false,
                    allowRemoveColumn: false,
                    outsideClickDeselects: false,
                    persistentState: true,
                    columns: buildColumns(settings.handsontable.columns[i]),
                    afterChange: function (changes, source) {
                        var columns = settings.handsontable.columns[i];
                        updateTable(name, table, container, columns);

                        // Ignore the change made by the loading of data
                        if (source != "loadData") {
                            triggerCallback(container)
                        }
                    }
                });
            })
        });
    }
};
