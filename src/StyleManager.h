#ifndef StyleManagerH
#define StyleManagerH
//============================================================================
/// \file   StyleManager.h
/// \author Uwe Kindler
/// \date   13.12.2021
/// \brief  Declaration of CStyleManager class
//============================================================================

//============================================================================
//                                   INCLUDES
//============================================================================
#include <QString>
#include <QVector>
#include <QPair>
#include <QObject>

class QIcon;

namespace acss
{
struct StyleManagerPrivate;
using QStringPair = QPair<QString, QString>;

/**
 * Encapsulates all information about a single stylesheet based style
 */
class CStyleManager : public QObject
{
	Q_OBJECT
private:
	StyleManagerPrivate* d; ///< private data (pimpl)
	friend struct StyleManagerPrivate;

public:
	enum eError
	{
		NoError,
		CssTemplateError,
		CssExportError,
		ThemeXmlError,
		StyleJsonError,
		ResourceGeneratorError,
	};

	enum eLocation
	{
		ThemesLocation,
		ResourceTemplatesLocation,
		FontsLocation
	};

	/**
	 * Default Constructor
	 */
	CStyleManager(QObject* parent = nullptr);

	/**
	 * Virtual Destructor
	 */
	virtual ~CStyleManager();

	/**
	 * Set the directory path that contains all styles
	 */
	void setStylesDirPath(const QString& DirPath);

	/**
	 * Returns the set styles dir
	 */
	QString stylesDirPath() const;

	/**
	 * Returns the current style
	 */
	QString currentStyle() const;

	/**
	 * Returns the absolute path of the current style.
	 * If your styles stylesDirPath() is C:/styles and your current style is
	 * qt_material then this function returns C:/styles/qt_material
	 */
	QString currentStylePath() const;

	/**
	 * Returns the list of available styles in the given styles directory
	 */
	const QStringList& styles() const;

	/**
	 * Returns a list of all themes for the current style.
	 * If no style has been set, this function returns an empty list
	 */
	const QStringList& themes() const;

	/**
	 * Returns a list of all theme variables for colors
	 */
	const QMap<QString, QString>& themeColorVariables() const;

	/**
	 * Returns the absolute dir path for the given location
	 */
	QString path(eLocation Location) const;

	/**
	 * Returns the absolute output dir path where the generated styles will
	 * get stored.
	 * \see setOutputDirPath()
	 */
	QString outputDirPath() const;

	/**
	 * Sets the output directory path where the generated theme will be stored
	 */
	void setOutputDirPath(const QString& Path);

	/**
	 * Returns the output path for the current style.
	 * The output path is the outputDirPath() + the style name.
	 * If your output path is C:/temp/styles and your style is qt_material
	 * then this functions returns C:/temp/styles/qt_material
	 */
	QString currentStyleOutputPath() const;

	/**
	 * Returns the value for the given theme variable.
	 * For example themeVariable("primaryColor") may return "#ac2300".
	 * Returns an empty string, if the given theme variable does not exist.
	 */
	QString themeVariableValue(const QString& VariableId) const;

	/**
	 * You can use this function to add or overwrite a theme variable.
	 * You may use this function to add application specific colors that are not
	 * defined in the current style.
	 * If you changed a theme variable or a number of theme variables then you
	 * should call updateStylesheet() to request a reprocessing of the style
	 * template and to update the stylesheet.
	 */
	void setThemeVariableValue(const QString& VariableId, const QString& Value);

	/**
	 * Returns the color for the given VariableId.
	 * If VariableId is not a color variable, then this function returns an invalid
	 * QColor.
	 */
	QColor themeColor(const QString& VariableId) const;

	/**
	 * Returns the current set theme
	 */
	QString currentTheme() const;

	/**
	 * Returns the processed style stylesheet.
	 * If the style or the theme of a style changed, you can read the new
	 * stylesheet from this function
	 */
	QString styleSheet() const;

	/**
	 * This function replaces the style variables in the given template with
	 * the value of the registered style variables.
	 * You can use this function if you have split your stylesheet into
	 * several parts or if you use plugins that provide additional stylesheets
	 * to get the processed stylesheed if the theme changed.
	 * Returns the final stylesheet or an empty string in case of an error.
	 * If the OutputFile parameter is given, the generated stylesheet file
	 * will be stored into the currentStyleOutputPath() folder with the given
	 * OutputFile name.
	 */
	QString processStylesheetTemplate(const QString& Template, const QString& OutputFile = QString());

	/**
	 * Returns the icon for the current style or an empty icon if the style does
	 * not provide an icon
	 */
	const QIcon& styleIcon() const;

	/**
	 * Returns the error state of the object.
	 */
	eError error() const;

	/**
	 * Returns a string describing the last error that occured
	 */
	QString errorString() const;

	/**
	 * This function creates a palette with the theme colors of the currently
	 * selected theme
	 */
	QPalette generateThemePalette() const;

	/**
	 * Read access to the Json object with all stlye parameters
	 */
	const QJsonObject& styleParameters() const;


public slots:
	/**
	 * Sets the theme to use.
	 * Use the theme name without the file extension. That means, if your
	 * theme file id dark_cyan.xml then set the theme to `dark_cyan'
	 * Returns true on success and false on error.
	 * Changing the theme does not trigger a stylesheet update. So after
	 * changing the theme, you can add or overwrite theme variables
	 * via setThemeVariableValue()
	 * To trigger processing of the style template, you need to call
	 * explicitely updateStylesheet().
	 */
	bool setCurrentTheme(const QString& Theme);

	/**
	 * Set the current style
	 */
	bool setCurrentStyle(const QString& Style);

	/**
	 * Call this function if you would like to reprocess the style template.
	 * Call this function, if you have changed the theme or some theme variables
	 * via setThemeVariable() to request an update of the stylesheet.
	 * The function updates the application palette with the theme colors
	 * (updateApplicationPaletteColors()), generates the theme SVG rseouces
	 * (generateResources()) and then generates the stylesheet if it has a
	 * template file.
	 * If you have split your stylesheet into several pieces or if your would
	 * like to separate the update of the resources and the generation of
	 * the stylehseets, then you should not use this function. Instead, call
	 * processStyleTemplate() to generate the SVG resources and update the
	 * palette an then use processStylesheetTemplate() to generate the style
	 * sheets.
	 */
	bool updateStylesheet();

	/**
	 * Call this function, if you would like to update the SVG files and the
	 * application palette. The function calls generateResources() and
	 * updateApplicationPaletteColors().
	 * The functio will not update or create any stylesheet. After calling this
	 * function, you can use the processStylesheetTemplate() function to
	 * manually generate the style sheets from stylesheet template files.
	 */
	bool processStyleTemplate();

	/**
	 * Generate the required icons for this theme.
	 * You can call this function, if you changed some theme settings or theme
	 * variables and if you would like to recreate the svg resources of this
	 * style.If you call updateStylesheet(), then this function will be
	 * called automatically
	 */
	bool generateResources();

	/**
	 * Update the palette colors with the colors read from json file.
	 * This function is called automatically if updateStylesheet() is called.
	 * The function creates a palette with theme colors via generateThemePalette()
	 * and then assigns the platte to the application object.
	 */
	void updateApplicationPaletteColors();

signals:
	/**
	 * This signal is emitted if the selected style changed
	 */
	void currentStyleChanged(const QString& Style);

	/**
	 * This signal is emitted, if the selected theme in a style changed
	 */
	void currentThemeChanged(const QString& Theme);

	/**
	 * This signal is emitted if the stylesheet changed.
	 * The stylecheed changes if the style changes, the theme changes or if a
	 * style variable changed an the user requested a styleheet update
	 */
	void stylesheetChanged();
}; // class StyleManager
}
 // namespace namespace_name
//-----------------------------------------------------------------------------
#endif // StyleManagerH

