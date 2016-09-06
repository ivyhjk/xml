<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test <value> tag class implementation.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class ValueTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Get an xml for tests.
     *
     * @param Vector<mixed> $parameters The parameters to send.
     *
     * @return string
     */
    private function getXml(Vector<mixed> $parameters) : string
    {
        $document = new DOMDocument();

        $value = new Value($parameters, $document);

        $element = $value->getElement();

        $document->appendChild($element);

        $xml = $document->saveXML();

        return preg_replace('/\n/', '', $xml);
    }

    /**
     * Test getElement method as string value.
     *
     * @return void
     */
    public function testGetElementString() : void
    {
        $expectedXml = '<?xml version="1.0"?><value><string>foo</string></value>';

        static::assertEquals($expectedXml, $this->getXml(Vector{'foo'}));
    }

    /**
     * Test getElement method as integer value.
     *
     * @return void
     */
    public function testGetElementInteger() : void
    {
        $expectedXml = '<?xml version="1.0"?><value><int>1337</int></value>';

        static::assertEquals($expectedXml, $this->getXml(Vector{1337}));
    }

    /**
     * Test getElement method as float value.
     *
     * @return void
     */
    public function testGetElementFloat() : void
    {
        $expectedXml = '<?xml version="1.0"?><value><double>13.37</double></value>';

        static::assertEquals($expectedXml, $this->getXml(Vector{13.37}));
    }

    /**
     * Test getElement method as double value.
     *
     * @return void
     */
    public function testGetElementDouble() : void
    {
        $expectedXml = '<?xml version="1.0"?><value><double>1.0E+20</double></value>';

        static::assertEquals($expectedXml, $this->getXml(Vector{1.0E+20}));
    }

    /**
     * Test tjhe getElement method as struct value.
     *
     * @return void
     */
    public function testGetElementStruct() : void
    {
        $expectedXml = '<?xml version="1.0"?><value><struct><member><name>foo</name><value><string>bar</string></value></member><member><name>bar</name><value><string>baz</string></value></member></struct></value>';

        $values = Vector{
            Map{
                'foo' => 'bar',
                'bar' => 'baz'
            }
        };

        static::assertEquals($expectedXml, $this->getXml($values));
    }

    /**
     * Test the fromNode error.
     *
     * @return void
     */
    public function testFromNodeError() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        Value::fromNode($node, new DOMDocument());
    }

    /**
     * Test the correct workflow for fromNode method  when value has a string type.
     *
     * @return void
     */
    public function testFromNodeString() : void
    {
        $node = new SimpleXMLElement('
            <value>
                <string>foo</string>
            </value>
        ');

        $entity = Value::fromNode($node, new DOMDocument());

        $expected = Vector{
            'foo'
        };

        static::assertEquals($expected, $entity->getValues());
    }

    /**
     * Test the correct workflow for fromNode method  when value has a integer type.
     *
     * @return void
     */
    public function testFromNodeInteger() : void
    {
        $node = new SimpleXMLElement('
            <value>
                <int>1337</int>
            </value>
        ');

        $entity = Value::fromNode($node, new DOMDocument());

        $expected = Vector{
            1337
        };

        static::assertEquals($expected, $entity->getValues());
    }

    /**
     * Test the correct workflow for fromNode method when
     * the value has struct type value.
     *
     * @return void
     */
    public function testFromNodeStruct() : void
    {
        $node = new SimpleXMLElement('
            <value>
                <struct>
                    <member>
                        <name>foo</name>
                        <value>
                            <string>bar</string>
                        </value>
                    </member>
                    <member>
                        <name>bar</name>
                        <value>
                            <string>baz</string>
                        </value>
                    </member>
                </struct>
            </value>
        ');

        $entity = Value::fromNode($node, new DOMDocument());

        $this->assertCount(1, $entity->getValues());
    }

    /**
     * Test the parseValue method correct workflow when a struct is given.
     *
     * @depends testFromNodeStruct
     * @return void
     */
    public function testParseValueStruct() : void
    {
        $node = new SimpleXMLElement('
            <value>
                <struct>
                    <member>
                        <name>foo</name>
                        <value>
                            <string>bar</string>
                        </value>
                    </member>
                    <member>
                        <name>bar</name>
                        <value>
                            <string>baz</string>
                        </value>
                    </member>
                </struct>
            </value>
        ');

        $expectedValue = Map{
            'foo' => 'bar',
            'bar' => 'baz'
        };

        $entity = Value::fromNode($node, new DOMDocument());

        $parsed = Value::parseValue($entity);

        static::assertEquals($expectedValue, $parsed);
    }

    /**
     * Test the parseValue method correct
     * workflow when a nested struct is given.
     *
     * @depends testParseValueStruct
     * @return void
     */
    public function testParseValueNestedStruct() : void
    {
        $node = new SimpleXMLElement('
            <value>
                <struct>
                    <member>
                        <name>foo</name>
                        <value>
                            <string>bar</string>
                        </value>
                    </member>
                    <member>
                        <name>bar</name>
                        <value>
                            <string>baz</string>
                        </value>
                    </member>
                    <member>
                        <name>baz</name>
                        <value>
                            <struct>
                                <member>
                                    <name>zzz</name>
                                    <value>
                                        <string>xxx</string>
                                    </value>
                                </member>
                                <member>
                                    <name>ccc</name>
                                    <value>
                                        <struct>
                                            <member>
                                                <name>vvv</name>
                                                <value>
                                                    <struct>
                                                        <member>
                                                            <name>bbb</name>
                                                            <value>
                                                                <string>nnn</string>
                                                            </value>
                                                        </member>
                                                    </struct>
                                                </value>
                                            </member>
                                        </struct>
                                    </value>
                                </member>
                            </struct>
                        </value>
                    </member>
                </struct>
            </value>
        ');

        $expectedValue = Map{
            'foo' => 'bar',
            'bar' => 'baz',
            'baz' => Map{
                'zzz' => 'xxx',
                'ccc' => Map{
                    'vvv' => Map{
                        'bbb' => 'nnn'
                    }
                }
            }
        };

        $entity = Value::fromNode($node, new DOMDocument());

        $parsed = Value::parseValue($entity);

        static::assertEquals($expectedValue, $parsed);
    }

    /**
     * Test parseValues method (multiple Value instances).
     *
     * @depends testParseValueStruct
     * @return void
     */
    public function testParseValues() : void
    {
        $firstNode = new SimpleXMLElement('
            <value>
                <struct>
                    <member>
                        <name>foo</name>
                        <value>
                            <string>bar</string>
                        </value>
                    </member>
                    <member>
                        <name>bar</name>
                        <value>
                            <string>baz</string>
                        </value>
                    </member>
                </struct>
            </value>
        ');

        $secondNode = new SimpleXMLElement('
            <value>
                <struct>
                    <member>
                        <name>baz</name>
                        <value>
                            <string>bar</string>
                        </value>
                    </member>
                    <member>
                        <name>bar</name>
                        <value>
                            <string>foo</string>
                        </value>
                    </member>
                </struct>
            </value>
        ');

        $firstValue = Value::fromNode($firstNode, new DOMDocument());
        $secondValue = Value::fromNode($secondNode, new DOMDocument());

        $values = Vector{
            $firstValue,
            $secondValue
        };

        $parsed = Value::parseValues($values);

        $expected = Vector{
            Map{
                'foo' => 'bar',
                'bar' => 'baz'
            },
            Map{
                'baz' => 'bar',
                'bar' => 'foo'
            }
        };

        static::assertEquals($expected, $parsed);
    }
}
