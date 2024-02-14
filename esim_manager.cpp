#include <gtk/gtk.h>
#include <iostream>
#include <string>

GtkWidget* output_window; // Declaring output_window at a global scope

static void onConfirmButtonClicked(GtkWidget* widget, gpointer data) {
    GtkWidget* profileWindow = static_cast<GtkWidget*>(data);
    gchar* message = g_strdup_printf("Inside the close function\n");
    if (profileWindow != nullptr) {
        gtk_widget_destroy(profileWindow);
    }
}

static void onProfileButtonClicked(GtkWidget* widget, gpointer data) {
    const gchar* label = gtk_button_get_label(GTK_BUTTON(widget));
    int profileIndex = atoi(label + 7); // Extract the profile number from the label

    GtkWidget* confirmationWindow = gtk_message_dialog_new(NULL,
                                                           GTK_DIALOG_MODAL,
                                                           GTK_MESSAGE_QUESTION,
                                                           GTK_BUTTONS_YES_NO,
                                                           "Are you sure you want to select Profile_%d?", profileIndex);

    gtk_window_set_title(GTK_WINDOW(confirmationWindow), "Confirm Profile Selection");
    gint result = gtk_dialog_run(GTK_DIALOG(confirmationWindow));
    if (result == GTK_RESPONSE_YES) {
        // Confirmation to select profile accepted, perform further actions if needed
        // For example, update the output or perform additional tasks
        gchar* message = g_strdup_printf("Selected Profile_%d.\n", profileIndex);
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window)), &iter);
        gtk_text_buffer_insert(gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window)), &iter, message, -1);
        g_free(message);
    }

    gtk_widget_destroy(confirmationWindow);
}

void handleProfileSelection(const gchar* action, GtkTextBuffer* buffer, int profileIndex) {
    GtkWidget* profileWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(profileWindow), "Select Profile");
    gtk_container_set_border_width(GTK_CONTAINER(profileWindow), 40);
    gtk_widget_set_size_request(profileWindow, 600, 400);
    gtk_window_set_resizable(GTK_WINDOW(profileWindow), FALSE);

    GtkWidget* profileBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(profileWindow), profileBox);

    GtkWidget* profileLabel = gtk_label_new(("Are you sure you want to select Profile_" + std::to_string(profileIndex)).c_str());
    gtk_box_pack_start(GTK_BOX(profileBox), profileLabel, TRUE, TRUE, 0);

    GtkWidget* confirmButton = gtk_button_new_with_label("Confirm");
    gtk_box_pack_start(GTK_BOX(profileBox), confirmButton, TRUE, TRUE, 0);

        gchar* message = g_strdup_printf("Inside the handleProfileSelection function\n");
    g_signal_connect(confirmButton, "clicked", G_CALLBACK(gtk_widget_destroyed), &profileWindow);
    g_signal_connect(profileWindow, "destroy", G_CALLBACK(gtk_widget_destroyed), &profileWindow);

    gtk_widget_show_all(profileWindow);
}

static void button_clicked(GtkWidget* widget, gpointer data) {
    const gchar* label = gtk_button_get_label(GTK_BUTTON(widget));

    if (std::string(label) == "Enable Profile" || std::string(label) == "Disable Profile" || std::string(label) == "Delete Profile" || std::string(label) == "Set Nickname" || std::string(label) == "Profile Information") {
        GtkWidget* profileSelectionWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(profileSelectionWindow), "Select Profile");
        gtk_container_set_border_width(GTK_CONTAINER(profileSelectionWindow), 40);
        gtk_widget_set_size_request(profileSelectionWindow, 300, 200);
        gtk_window_set_resizable(GTK_WINDOW(profileSelectionWindow), FALSE);

        GtkWidget* profileBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(profileSelectionWindow), profileBox);

        GtkWidget* profileButtons[10];

        for (int i = 0; i < 10; ++i) {
            gchar* buttonText = g_strdup_printf("Profile %d", i);
            profileButtons[i] = gtk_button_new_with_label(buttonText);
            g_signal_connect(profileButtons[i], "clicked", G_CALLBACK(onProfileButtonClicked), NULL);
            gtk_box_pack_start(GTK_BOX(profileBox), profileButtons[i], TRUE, TRUE, 0);
            g_free(buttonText);
        }

        gtk_widget_show_all(profileSelectionWindow);
    } else if (std::string(label) == "Read eSIM ID") {
        // Handle Read eSIM ID button
        gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window)), "Reading eSIM ID...\n", -1);
    } else if (std::string(label) == "Download Profile") {
        // Handle Download Profile button
        gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window)), "Downloading Profile...\n", -1);
    } else if (std::string(label) == "Display Profiles") {
        // Handle Display Profiles button
        gtk_text_buffer_insert_at_cursor(gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window)), "Displaying Profiles...\n", -1);
    }
    // Add more else if conditions for other buttons if needed
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "eSIM Profile Manager");
    gtk_container_set_border_width(GTK_CONTAINER(window), 40);

//    gtk_widget_set_size_request(window, 800, 800);
    // Set the initial size of the window based on the screen's width and height
    GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(window));
    gint screenWidth = gdk_screen_get_width(screen);
    gint screenHeight = gdk_screen_get_height(screen);
    gtk_widget_set_size_request(window, screenWidth / 2, screenHeight / 2);
    
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    const gchar* button_labels[] = {
        "Read eSIM ID", "Download Profile", "Enable Profile",
        "Disable Profile", "Delete Profile", "Set Nickname",
        "Display Profiles", "Profile Information"
    };

    GtkWidget* button_labels_widgets[8];
    GtkWidget* buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), buttonBox, FALSE, FALSE, 0);

    for (int i = 0; i < 8; ++i) {
        if (strcmp(button_labels[i], "") == 0) {
            GtkWidget* gapLabel = gtk_label_new("");
            gtk_widget_set_size_request(gapLabel, screenWidth / 100, screenHeight / 200); // Adjust the size as needed
            gtk_container_add(GTK_CONTAINER(buttonBox), gapLabel);
        } else {
            button_labels_widgets[i] = gtk_button_new_with_label(button_labels[i]);
            g_signal_connect(button_labels_widgets[i], "clicked", G_CALLBACK(button_clicked), NULL);
            gtk_container_add(GTK_CONTAINER(buttonBox), button_labels_widgets[i]);
        }
    }

    output_window = gtk_text_view_new();
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(output_window));
    gtk_text_buffer_set_text(buffer, "Output will appear here\n", -1);
    GtkWidget* scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), output_window);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, 0);
    gtk_widget_set_size_request(scrolledWindow, -1, screenHeight / 4);

    // Apply CSS to the window for background color and button styling
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "window { background-color: #34495e; }"
        "button { padding: 10px; font-size: 15px; color: #000000; font-weight: bold; background-color: #3498db; border-radius: 15px; }",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

